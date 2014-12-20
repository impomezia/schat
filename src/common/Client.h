/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

#ifndef CLIENT_H_
#define CLIENT_H_

#include <QSslSocket>

#include "interfaces/IClient.h"
#include "schat.h"

class SCHAT_EXPORT Client : public QSslSocket, public IClient
{
  Q_OBJECT

public:
  Client(QObject *parent = 0);
  void leave(int status = 1001) override;
  void open(const QString &hostName, quint16 port = 7665) override;
  void ping() override;
  void reconnect() override;
  void send(const SJMPPacket &packet) override;

  void addListener(IClientListener *listener) override;
  void removeListener(IClientListener *listener) override;

# if QT_VERSION >= 0x050000
  void connectToHost(const QString &hostName, quint16 port, OpenMode openMode = ReadWrite, NetworkLayerProtocol protocol = AnyIPProtocol);
# endif

protected slots:
  void connectToHostImplementation(const QString &hostName, quint16 port, OpenMode openMode = ReadWrite);

private slots:
  void onDisconnected();
  void onEncrypted();
  void onError(QAbstractSocket::SocketError socketError);
  void onReadyRead();
  void onSslErrors(const QList<QSslError> &errors);

private:
  qint32 getPayloadSize();
  void release();

  bool m_release;
  int m_status;
  qint32 m_nextFrameSize;
  QList<IClientListener*> m_listeners;
};

#endif // CLIENT_H_
