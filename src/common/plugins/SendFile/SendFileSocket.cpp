/* $Id: SendFileSocket.cpp 2776 2012-06-16 05:21:07Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#include <QBasicTimer>
#include <QTimerEvent>
#include <QFile>

#include "debugstream.h"
#include "net/SimpleID.h"
#include "SendFileSocket.h"
#include "SendFileTransaction.h"

#define DISCOVERY_TIMEOUT 15000
#define HANDSHAKE_TIMEOUT 15000
#define RECONNECT_TIMEOUT 1000
#define SENDFILE_CHUNK 1048576

namespace SendFile {


Socket::Socket(QObject *parent)
  : QTcpSocket(parent)
  , m_release(false)
  , m_serverSide(true)
  , m_role(UnknownRole)
  , m_mode(HandshakeMode)
  , m_file(0)
  , m_size(0)
  , m_port(0)
  , m_nextBlockSize(0)
{
  init();
  setMode(HandshakeMode);
}


Socket::Socket(const QString& host, quint16 port, const QByteArray &id, int role, QObject *parent)
  : QTcpSocket(parent)
  , m_release(false)
  , m_serverSide(false)
  , m_role(UnknownRole)
  , m_mode(UnknownMode)
  , m_id(id)
  , m_file(0)
  , m_size(0)
  , m_host(host)
  , m_port(port)
  , m_nextBlockSize(0)
{
  role ? m_role = ReceiverSlave : m_role = SenderSlave;

  init();
  discovery();
}


Socket::~Socket()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::~Socket()" << this)

  if (m_timer->isActive())
    m_timer->stop();

  delete m_timer;
}


bool Socket::reconnect()
{
  if (m_mode == DiscoveringMode || m_mode == HandshakeMode) {
    m_timer->start(RECONNECT_TIMEOUT, this);
    return true;
  }

  return false;
}


void Socket::accept()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::accept()" << this)

  qint32 size = 1;
  QByteArray data;

  data.reserve(size + 4);
  data.append((char*)&size, 4);
  data.append('A');

  write(data);
}


void Socket::acceptSync()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::acceptSync()" << this)

  if (state() != ConnectedState)
    return;

  qint32 size = 1;
  QByteArray data;

  data.reserve(size + 4);
  data.append((char*)&size, 4);
  data.append('s');

  write(data);
  setMode(DataMode);
}


/*!
 * Принудительное отключение сокета.
 */
void Socket::leave()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::leave(), state:" << state() << " mode:" << mode() << this);
  if (m_release)
    return;

  disconnect(SIGNAL(readyRead()));
  setMode(UnknownMode);

  if (state() == ConnectedState) {
    flush();
    disconnectFromHost();
    if (m_port && state() != UnconnectedState) {
      if (!waitForDisconnected(1000))
        abort();
    }
  }
  else {
    abort();
    disconnected();
  }
}


/*!
 * Отказ в авторизации.
 */
void Socket::reject()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::reject()" << this)

  if (state() != ConnectedState)
    return;

  qint32 size = 1;
  QByteArray data;

  data.reserve(size + 4);
  data.append((char*)&size, 4);
  data.append('R');

  write(data);
  leave();
}


/*!
 * Установка файла для записи или чтения.
 *
 * \param file Указатель на файл.
 * \param size Размер файла.
 */
void Socket::setFile(QFile *file, qint64 size)
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::setFile(), fileName:" << file->fileName() << " size:" << size << this)
  m_file = file;
  m_size = size;
}


void Socket::sync()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::sync()" << this)

  qint32 size = 1;
  QByteArray data;

  data.reserve(size + 4);
  data.append((char*)&size, 4);
  data.append('S');

  write(data);
}


void Socket::timerEvent(QTimerEvent *event)
{
  if (event->timerId() != m_timer->timerId()) {
    QTcpSocket::timerEvent(event);
    return;
  }

  if (m_mode == DiscoveringMode || m_mode == HandshakeMode) {
    if (state() != UnconnectedState) {
      abort();
      setSocketError(QAbstractSocket::SocketTimeoutError);
      setSocketState(QAbstractSocket::UnconnectedState);
      disconnected();
    }
    else
      discovery();
  }
}


/*!
 * Обработка подключения.
 */
void Socket::connected()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::connected()" << this)

# if QT_VERSION >= 0x040600
  setSocketOption(QAbstractSocket::KeepAliveOption, 1);
# endif

  if (m_mode == DiscoveringMode) {
    setMode(HandshakeMode);

    qint32 size = 23;
    QByteArray hello;

    hello.reserve(size + 4);
    hello.append((char*)&size, 4);
    hello.append('H');
    hello.append(m_role);
    hello.append(m_id);

    write(hello);
  }
}


void Socket::disconnected()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::disconnected(), m_release:" << m_release << this)
  if (m_release)
    return;

  m_release = true;
  m_timer->stop();

  emit released();
}


void Socket::discovery()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::discovery(), host:" << m_host << ", port:" << m_port << this)
  if (!m_port)
    return;

  m_timer->stop();
  m_release = false;

  setMode(DiscoveringMode);
  connectToHost(m_host, m_port);
}


void Socket::error(QAbstractSocket::SocketError socketError)
{
  Q_UNUSED(socketError)
  SCHAT_DEBUG_STREAM("[SendFile] Socket::error()" << socketError << errorString() << this)

  if (state() != ConnectedState)
    disconnected();
}


void Socket::readyRead()
{
  forever {
    if (m_release)
      return;

    if (m_mode == DataMode && (m_role == ReceiverMaster || m_role == ReceiverSlave)) {
      qint64 bytes = bytesAvailable();
      if (!bytes)
        break;

      qint64 pos = m_file->pos();
      if (pos + bytes > m_size)
        bytes = m_size - pos;

      m_file->write(read(bytes));
      progress(pos + bytes);
      return;
    }

    if (!m_nextBlockSize) {
      if (bytesAvailable() < 4)
        break;

      read((char*)&m_nextBlockSize, 4);
    }

    if (bytesAvailable() < m_nextBlockSize)
      break;

    readPacket();

    if (m_nextBlockSize) {
      read(m_nextBlockSize);
      m_nextBlockSize = 0;
    }
  }
}


void Socket::init()
{
  SCHAT_DEBUG_STREAM("[SendFile] Socket::init()" << this)

  m_timer = new QBasicTimer();

  connect(this, SIGNAL(connected()), SLOT(connected()));
  connect(this, SIGNAL(disconnected()), SLOT(disconnected()));
  connect(this, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(error(QAbstractSocket::SocketError)));
  connect(this, SIGNAL(readyRead()), SLOT(readyRead()));
}


/*!
 * Обновление прогресса записи в файл.
 *
 * Высылается сигнал с информацией о записанной позиции \p pos,
 * отправителю отсылается пакет c этой же информацией, для корректного формирования прогресса и отправки новых данных.
 * Если позиция равна размеру файла, сокет и файл закрываются.
 *
 * \param pos Последняя записанная позиция.
 */
void Socket::progress(qint64 pos)
{
  emit progress(pos, m_size);

  qint32 size = 9;
  QByteArray data;

  data.reserve(size + 4);
  data.append((char*)&size, 4);
  data.append('P');
  data.append((char*)&pos, 8);

  write(data);

  if (pos == m_size) {
    m_file->close();
    leave();
    emit finished();
  }
}


/*!
 * Установка режима работы сокета.
 */
void Socket::setMode(Mode mode)
{
  if (mode == DiscoveringMode) {
    m_timer->start(DISCOVERY_TIMEOUT, this);
  }
  else if (mode == HandshakeMode) {
    m_timer->start(HANDSHAKE_TIMEOUT, this);
  }
  else if (mode == DataMode) {
    m_timer->stop();
  }

  m_mode = mode;
}


void Socket::readPacket()
{
  char code;
  getChar(&code);
  m_nextBlockSize--;

  if (code == 'H' && m_nextBlockSize >= 22) {
    m_nextBlockSize -= 22;
    char role;
    getChar(&role);
    m_id = read(21);
    if (SimpleID::typeOf(m_id) == SimpleID::MessageId && m_serverSide) {
      SCHAT_DEBUG_STREAM("[SendFile] Socket::readPacket(), code:" << code << "role:" << role << this)
      emit handshake(m_id, role);
    }
  }
  else if (code == 'S') {
    emit syncRequest();
  }
  else if (code == 's') {
    setMode(DataMode);
    sendBlock();
  }
  else if (code == 'R') {
    leave();
    setMode(DiscoveringMode);
  }
  else if (code == 'A') {
    emit acceptRequest();
  }
  else if (code == 'P' && m_nextBlockSize >= 8) {
    m_nextBlockSize -= 8;
    qint64 pos;
    read((char*)&pos, 8);

    if (m_file->pos() - pos < SENDFILE_CHUNK)
      sendBlock();

    emit progress(pos, m_size);

    if (pos == m_size) {
      leave();
      emit finished();
    }
  }
}


void Socket::sendBlock()
{
  if (m_file->atEnd())
    return;

  if (m_file->bytesAvailable() < SENDFILE_CHUNK)
    write(m_file->readAll());
  else
    write(m_file->read(SENDFILE_CHUNK));
}

} // namespace SendFile
