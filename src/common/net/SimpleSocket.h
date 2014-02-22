/* $Id: SimpleSocket.h 3714 2013-06-26 00:57:48Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#ifndef SIMPLESOCKET_H_
#define SIMPLESOCKET_H_

#if defined(SCHAT_NO_SSL)
  #include <QTcpSocket>
  #define QSslSocket QTcpSocket
#else
  #include <QSslSocket>
#endif

#include "net/Protocol.h"
#include "schat.h"

class SimpleSocketPrivate;

/*!
 * Наследник QTcpSocket выполняющий все операции на транспортном уровне протокола чата.
 */
class SCHAT_EXPORT SimpleSocket : public QSslSocket
{
  Q_OBJECT

public:
  /// Состояние таймера.
  enum TimerState {
    WaitingConnect,   ///< Ожидание установки соединения.
    WaitingHandshake, ///< Ожидание авторизации.
    Idling,           ///< Ожидание активности.
    WaitingReply,     ///< Ожидания ответа на пакет.
    Leaving           ///< Состояние завершения соединения.
  };

  explicit SimpleSocket(QObject *parent = 0);
  ~SimpleSocket();
  bool isAuthorized() const;
  bool send(const QByteArray &packet);
  bool send(const QList<QByteArray> &packets);
  bool setSocketDescriptor(int socketDescriptor);
  const QByteArray& channelId() const;
  inline bool isReady() const { if (state() == ConnectedState) return true; return false; }
  int version() const;
  QByteArray readBuffer() const;
  QDataStream *sendStream();
  qint64 date() const;
  quint64 id() const;
  quint64 rx() const;
  quint64 tx() const;
  void leave();
  void setAuthorized(const QByteArray &userId);
  void setDate(qint64 date);
  void setId(quint64 id);

# if QT_VERSION >= 0x050000
  void connectToHost(const QString & hostName, quint16 port, OpenMode openMode = ReadWrite, NetworkLayerProtocol protocol = AnyIPProtocol);
# endif

signals:
  void allDelivered(quint64 id);
  void newPackets(quint64 id, const QList<QByteArray> &packets);
  void released(quint64 id);
  void requestAuth(quint64 id);

protected:
  SimpleSocket(SimpleSocketPrivate &dd, QObject *parent);
  SimpleSocketPrivate *const d_ptr;
  virtual void newPacketsImpl();
  void timerEvent(QTimerEvent *event);

protected slots:
  void connectToHostImplementation(const QString &hostName, quint16 port, OpenMode openMode = ReadWrite);

private slots:
  void connected();
  void disconnected();
  void error(QAbstractSocket::SocketError socketError);
  void readyRead();

  #if !defined(SCHAT_NO_SSL)
  void encrypted();
  void checkSslErrors(const QList<QSslError> &errors);
  #endif

private:
  Q_DECLARE_PRIVATE(SimpleSocket);
};

#endif /* SIMPLESOCKET_H_ */
