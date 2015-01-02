/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtEndian>

#include "Client.h"
#include "interfaces/IClientListener.h"
#include "SJMPPacket.h"

Client::Client(QObject *parent)
  : QSslSocket(parent)
  , m_ready(false)
  , m_release(false)
  , m_status(1001)
  , m_nextFrameSize(0)
  , m_port(0)
{
# if QT_VERSION >= 0x050000
  setProtocol(QSsl::TlsV1_0);
# else
  setProtocol(QSsl::TlsV1);
# endif

  setSocketOption(LowDelayOption, 1);

  connect(this, SIGNAL(disconnected()), SLOT(onDisconnected()));
  connect(this, SIGNAL(encrypted()), SLOT(onEncrypted()));
  connect(this, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onError(QAbstractSocket::SocketError)));
  connect(this, SIGNAL(readyRead()), SLOT(onReadyRead()));
  connect(this, SIGNAL(sslErrors(QList<QSslError>)), SLOT(onSslErrors(QList<QSslError>)));
}


bool Client::isReady() const
{
  return m_ready;
}


void Client::leave(int status)
{
  m_status = status;

  if (state() == ConnectedState) {
    flush();
    disconnectFromHost();
    if (state() != UnconnectedState) {
      if (!waitForDisconnected(1000))
        abort();
    }
  }
  else {
    abort();
    release();
  }
}


void Client::open(const QString &hostName, quint16 port)
{
  m_hostName = hostName;
  m_port     = port;

  connectToHostEncrypted(hostName, port);
}


void Client::ping()
{
  putChar(0x0);
  putChar(0x9);
}


void Client::reconnect()
{
  if (!m_port)
    return;

  open(m_hostName, m_port);
}


void Client::send(const SJMPPacket &packet, bool queue)
{
  if (queue && !isReady()) {
    m_queue.enqueue(packet);
    return;
  }

  const QByteArray payload = packet.serialize();
  const quint32 size       = payload.size();

  if (size >= 254) {
    if (size > 65535) {
      putChar((unsigned) 255);

      const quint32 i = qToBigEndian(size);
      write((char*)&i, sizeof(i));
    }
    else {
      putChar((unsigned) 254);

      const quint16 i = qToBigEndian((quint16) size);
      write((char*)&i, sizeof(i));
    }
  }
  else {
    putChar(size);
  }

  putChar(0x1);
  write(payload);
}


void Client::setReady(bool ready)
{
  if (m_ready == ready)
    return;

  m_ready = ready;

  if (ready && !m_queue.isEmpty()) {
    while (!m_queue.isEmpty())
      send(m_queue.dequeue(), false);
  }
}


void Client::addListener(IClientListener *listener)
{
  if (!m_listeners.contains(listener))
    m_listeners.append(listener);
}


void Client::removeListener(IClientListener *listener)
{
  m_listeners.removeAll(listener);
}


#if QT_VERSION >= 0x050000
void Client::connectToHost(const QString & hostName, quint16 port, OpenMode openMode, NetworkLayerProtocol protocol)
{
  foreach (IClientListener *listener, m_listeners) {
    listener->onConnectToHost(this, hostName, port);
  }

  connectToHostImplementation(hostName, port, openMode);
  QSslSocket::connectToHost(hostName, port, openMode, protocol);
}
#endif


void Client::connectToHostImplementation(const QString &hostName, quint16 port, OpenMode openMode)
{
  m_release = false;
  m_status  = 1001;
  m_ready   = false;

# if QT_VERSION < 0x050000
  foreach (IClientListener *listener, m_listeners) {
    listener->onConnectToHost(this, hostName, port);
  }

  QSslSocket::connectToHostImplementation(hostName, port, openMode);
# else
  Q_UNUSED(hostName)
  Q_UNUSED(port)
  Q_UNUSED(openMode)
# endif
}


void Client::onDisconnected()
{
  release();
}


void Client::onEncrypted()
{
  foreach (IClientListener *listener, m_listeners) {
    listener->onAuthRequired(this);
  }
}


void Client::onError(QAbstractSocket::SocketError socketError)
{
  Q_UNUSED(socketError)

  release();
}


void Client::onReadyRead()
{
  forever {
    if (!m_nextFrameSize) {
      if (bytesAvailable() < 1)
        break;

      uchar size = 0;
      if (!getChar(reinterpret_cast<char*>(&size)))
        break;

      m_nextFrameSize = size;
    }

    if (m_nextFrameSize == 254) {
      if (bytesAvailable() < 2)
        break;

      quint16 size = 0;
      read(reinterpret_cast<char*>(&size), sizeof(size));
      m_nextFrameSize = qFromBigEndian(size);
    }
    else if (m_nextFrameSize == 255) {
      quint32 size = 0;
      read(reinterpret_cast<char*>(&size), sizeof(size));
      m_nextFrameSize = qFromBigEndian(size);
    }

    if (bytesAvailable() < m_nextFrameSize + 1)
      break;

    uchar opcode = 0;
    getChar(reinterpret_cast<char*>(&opcode));
    opcode &= ~0xf0;

    const QByteArray payload = read(m_nextFrameSize);

    if (opcode == 0x1 || opcode == 0x8) {
      SJMPPacket packet(payload);

      m_status = opcode == 0x8 ? 1000 : 1001;

      foreach (IClientListener *listener, m_listeners) {
        listener->onPacket(this, packet);
      }
    }
    else if (opcode == 0xA) {
      foreach (IClientListener *listener, m_listeners) {
        listener->onPong(this);
      }
    }

    m_nextFrameSize = 0;
  }
}


void Client::onSslErrors(const QList<QSslError> &errors)
{
  QList<QSslError::SslError> noCriticalErrors;
  noCriticalErrors.append(QSslError::NoError);
  noCriticalErrors.append(QSslError::SelfSignedCertificate);        // The passed certificate is self signed and the same certificate cannot be found in the list of trusted certificates.
  noCriticalErrors.append(QSslError::SelfSignedCertificateInChain); // The certificate chain could be built up using the untrusted certificates but the root could not be found locally.
  noCriticalErrors.append(QSslError::CertificateExpired);           // The certificate has expired: that is the notAfter date is before the current time.
  noCriticalErrors.append(QSslError::CertificateNotYetValid);       // The certificate is not yet valid: the notBefore date is after the current time.
  noCriticalErrors.append(QSslError::HostNameMismatch);

  for (int i = 0; i < errors.size(); ++i) {
    if (!noCriticalErrors.contains(errors.at(i).error()))
      return;
  }

  ignoreSslErrors();
}


qint32 Client::getPayloadSize()
{
  char c;

  if (!getChar(&c)) {
    return -1;
  }

  qint32 size = c;
  return size;
}


void Client::release()
{
  if (m_release)
    return;

  m_release = true;
  m_nextFrameSize = 0;

  foreach (IClientListener *listener, m_listeners) {
    listener->onDisconnected(this, m_status);
  }
}
