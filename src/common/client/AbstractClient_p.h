/* $Id: AbstractClient_p.h 3705 2013-06-23 02:24:46Z IMPOMEZIA $
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

#ifndef ABSTRACTCLIENT_P_H_
#define ABSTRACTCLIENT_P_H_

#include <QMap>

#include "Channel.h"
#include "client/AbstractClient.h"
#include "id/ChatId.h"
#include "net/SimpleSocket_p.h"

class AuthReply;
class ChatDNS;
class NetworkPool;
class PacketReader;

class AbstractClientPrivate : public SimpleSocketPrivate
{
  Q_DECLARE_PUBLIC(AbstractClient);

public:
  /// Результат выполнения функции authReply().
  enum AuthReplyAction {
    Nothing,       ///< Ничего не делать.
    ErrorState,    ///< Установить состояние AbstractClient::ClientError.
    WaitAuthState, ///< Установить состояние AbstractClient::WaitAuth.
    Setup,         ///< Успешное подключение к новому серверу.
    Restore        ///< Успешное подключение к предыдущему серверу.
  };

  AbstractClientPrivate();
  virtual ~AbstractClientPrivate();

  bool isSetup(const AuthReply &reply);
  QString mangleNick();
  QString serverName(const AuthReply &reply);
  virtual AuthReplyAction authReply(const AuthReply &reply);
  virtual void doneAuth(AuthReplyAction action);
  virtual void setClientState(AbstractClient::ClientState state);
  void startReconnectTimer();

  AbstractClient::ClientState clientState;   ///< Состояние клиента.
  AbstractClient::ClientState previousState; ///< Предыдущее состояние клиента.
  bool sendLock;                             ///< Блокировка отправки пакетов, пакеты будут добавлены в очередь и будут отправлены после снятия блокировки.
  ChatDNS *dns;                              ///< Объект для DNS запросов.
  ChatId authId;                             ///< Идентификатор авторизации.
  ChatId cookie;                             ///< Cookie.
  ChatId uniqueId;                           ///< Уникальный идентификатор пользователя.
  ClientChannel channel;                     ///< Канал пользователя.
  ClientChannel server;                      ///< Канал сервера.
  int collisions;                            ///< Счётчик коллизий ника.
  int reconnects;                            ///< Число попыток восстановить соединение.
  NetworkPool *pool;                         ///< Пул адресов серверов.
  PacketReader *reader;                      ///< Текущий объект PacketReader выполняющий чтение пакета.
  QBasicTimer *reconnectTimer;               ///< Таймер управляющий попытками переподключения.
  QList<QByteArray> sendQueue;               ///< Список виртуальных пакетов, ожидающих отправки если установлена блокировка на отправку.
  QString nick;                              ///< Оригинальный ник пользователя.
  QUrl url;                                  ///< Адрес, к которому будет подключен клиент.
  QVariantMap json;                          ///< Дополнительные данные.
};

#endif /* ABSTRACTCLIENT_P_H_ */
