/* $Id: ChannelMessage.h 3751 2013-07-13 03:03:07Z IMPOMEZIA $
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

#ifndef CHANNELMESSAGE_H_
#define CHANNELMESSAGE_H_

#include "messages/Message.h"
#include "net/packets/MessageNotice.h"

class MessageNotice;

class SCHAT_CORE_EXPORT ChannelMessage : public Message
{
public:
  static const QString kCommand;
  static const QString kDirection;
  static const QString kInternalId;
  static const QString kJSON;
  static const QString kMDate;
  static const QString kOID;
  static const QString kStatus;

  static const QString kIncoming;
  static const QString kOffline;
  static const QString kOutgoing;
  static const QString kReferring;
  static const QString kRejected;
  static const QString kRemoved;
  static const QString kUndelivered;

  ChannelMessage(MessagePacket packet);
  inline MessagePacket packet() const { return m_packet; }
  static bool isOffline(int status);

private:
  MessagePacket m_packet; ///< Пакет на основе которого формируется сообщение.
};

#endif /* CHANNELMESSAGE_H_ */
