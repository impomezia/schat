/* $Id: AbstractClient.h 3705 2013-06-23 02:24:46Z IMPOMEZIA $
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

#ifndef ABSTRACTCLIENT_H_
#define ABSTRACTCLIENT_H_

#include <QUrl>

#include "Channel.h"
#include "net/packets/Notice.h"
#include "net/SimpleSocket.h"
#include "schat.h"

class AbstractClientPrivate;
class ChatDNS;
class PacketReader;

class SCHAT_EXPORT AbstractClient : public SimpleSocket
{
  Q_OBJECT

public:
  /// Состояние клиента.
  enum ClientState {
    ClientOnline,      ///< Клиент успешно подключен
    ClientOffline,     ///< Клиент отключен.
    ClientConnecting,  ///< Клиент в состоянии подключения к серверу.
    ClientError,       ///< Критическая ошибка.
    ClientWaitAuth     ///< Ожидание действий пользователя, он должен выбрать метод авторизации.
  };

  /// Опции открытия адреса.
  enum OpenOptions {
    NoOptions = 0, ///< Нет опций.
    SaveUrl = 1    ///< Сохранить URL адрес как основной.
  };

  explicit AbstractClient(QObject *parent = 0);
  ~AbstractClient();

  bool openUrl(const QUrl &url, const ChatId &cookie = ChatId(), OpenOptions options = SaveUrl);

  bool send(const QByteArray &packet);
  bool send(const QList<QByteArray> &packets);
  bool send(Packet packet, bool echo = false);

  ChatDNS* dns();
  ClientChannel channel() const;
  ClientChannel server() const;
  ClientState clientState() const;
  ClientState previousState() const;
  const ChatId& cookie() const;
  const ChatId& uniqueId() const;
  const QString& nick() const;
  const QString& serverName() const;
  const QUrl& url() const;
  const QVariantMap& json() const;
  inline bool openUrl(const QString &url, const ChatId &cookie = ChatId(), OpenOptions options = SaveUrl) { return openUrl(QUrl(url), cookie, options); }
  PacketReader *reader();
  void lock();
  void setAuthId(const ChatId &id);
  void setNick(const QString &nick);
  void setUniqueId(const ChatId &id);
  void unlock();

  virtual void leave();

signals:
  void clientStateChanged(int state, int previousState);
  void idle();
  void packetReady(int type);
  void ready();
  void requestClientAuth();
  void restore();
  void setup();

protected:
  AbstractClient(AbstractClientPrivate &dd, QObject *parent);
  void newPacketsImpl();
  void timerEvent(QTimerEvent *event);

protected slots:
  virtual void requestAuth();
  void released();

private slots:
  void lookedUp();

private:
  Q_DECLARE_PRIVATE(AbstractClient);
};

#endif /* ABSTRACTCLIENT_H_ */
