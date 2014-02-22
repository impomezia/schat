/* $Id: MessagesImpl.h 3667 2013-05-29 23:02:50Z IMPOMEZIA $
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

#ifndef MESSAGESIMPL_H_
#define MESSAGESIMPL_H_

#include "client/ClientHooks.h"
#include "net/packets/MessageNotice.h"

class ChannelMessage;

namespace Hooks
{

class SCHAT_CORE_EXPORT MessagesImpl : public Messages
{
  Q_OBJECT

public:
  MessagesImpl(QObject *parent = 0);
  int read(MessagePacket packet);
  static bool ignored(ClientChannel user);
  void error(MessagePacket packet);
  void sent(MessagePacket packet);
  void unhandled(MessagePacket packet);

private slots:
  void clientStateChanged(int state, int previousState);
  void ready();

private:
  bool referring(const ChannelMessage &message) const;
  void rejectAll();

  QByteArray m_serverId;                          ///< Идентификатор последнего сервера.
  QMap<QByteArray, MessagePacket> m_undelivered; ///< Таблица сообщений, доставка которых не подтверждена.

};

} // namespace Hooks

#endif /* MESSAGESIMPL_H_ */
