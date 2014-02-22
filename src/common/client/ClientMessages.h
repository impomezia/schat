/* $Id: ClientMessages.h 3714 2013-06-26 00:57:48Z IMPOMEZIA $
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

#ifndef CLIENTMESSAGES_H_
#define CLIENTMESSAGES_H_

#include <QObject>
#include <QMultiMap>

#include "id/ChatId.h"
#include "net/packets/MessageNotice.h"

class ClientCmd;
class MessageNotice;
class SimpleClient;

namespace Hooks
{
  class Messages;
}

/*!
 * Базовый класс обрабатывающий сообщения пользователя.
 *
 * \sa Hooks::Messages.
 */
class SCHAT_EXPORT ClientMessages : public QObject
{
  Q_OBJECT

public:
  ClientMessages(QObject *parent = 0);
  inline void add(Hooks::Messages *hook)    { if (!m_hooks.contains(hook)) m_hooks.append(hook); }
  inline void remove(Hooks::Messages *hook) { m_hooks.removeAll(hook); }
  static bool isClientDate(int status);

  bool send(const QByteArray &dest, const QString &text);
  bool sendText(const QByteArray &dest, const QString &text, const QString &command = QString());
  void insert(MessageNotice *notice);

private slots:
  void channels(const QList<QByteArray> &channels);
  void notice(int type);

private:
  bool command(const QByteArray &dest, const ClientCmd &cmd);
  bool command(const QByteArray &dest, const QString &text, const QString &plain);
  void prepare(MessagePacket packet);
  void read(MessagePacket packet);
  void readText(MessagePacket packet);
  void sent(MessagePacket packet);

  ChatId m_randomId;                               ///< Случайный идентификатор для сообщений.
  int m_version;                                   ///< Версия протокола.
  QByteArray m_destId;                             ///< Текущий получатель сообщения.
  QList<Hooks::Messages*> m_hooks;                 ///< Хуки.
  QMultiMap<QByteArray, MessagePacket> m_pending;  ///< Сообщения отображение которых отложена, т.к. не известна информация об отправителе.
  SimpleClient *m_client;                          ///< Клиент чата.
};

#endif /* CLIENTMESSAGES_H_ */
