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

#ifndef CHANNELNOTICE_H_
#define CHANNELNOTICE_H_

#include "Channel.h"
#include "net/Channels.h"
#include "net/packets/Notice.h"

class ChannelNotice;
class PacketReader;
class PacketWriter;

typedef QSharedPointer<ChannelNotice> ChannelPacket;

class SCHAT_EXPORT ChannelNotice : public Notice
{
public:
  ChannelNotice(const QByteArray &sender, const QByteArray &dest, const QString &command, quint64 time = 0);
  ChannelNotice(quint16 type, PacketReader *reader);

  static ChannelPacket channel(ClientChannel channel, const QByteArray &dest, const QString &command = CHANNELS_CHANNEL_CMD);
  static ChannelPacket info(ClientChannel channel, qint64 date = 0);
  static ChannelPacket info(const QByteArray &user, const QList<QByteArray> &channels);
  static ChannelPacket reply(const ChannelNotice &source, int status);
  static ChannelPacket request(const QByteArray &user, const QByteArray &channel, const QString &command, const QString &text = QString());
  static ChannelPacket update(ClientChannel channel);

  QByteArray channelId;       ///< Идентификатор канала.
  quint8 gender;              ///< Пол и цвет пользователя.
  quint8 channelStatus;       ///< Базовый статус пользователя.
  QList<QByteArray> channels; ///< Список идентификаторов каналов, передаётся только для команды "channel".

protected:
  void write(PacketWriter *writer) const;
};



#endif /* CHANNELNOTICE_H_ */
