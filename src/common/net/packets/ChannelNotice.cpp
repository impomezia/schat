/* $Id: ChannelNotice.cpp 3632 2013-04-12 04:44:28Z IMPOMEZIA $
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

#include "DateTime.h"
#include "net/Channels.h"
#include "net/PacketReader.h"
#include "net/packets/ChannelNotice.h"
#include "net/PacketWriter.h"
#include "sglobal.h"

ChannelNotice::ChannelNotice(const QByteArray &sender, const QByteArray &dest, const QString &command, quint64 time)
  : Notice(sender, dest, command, time)
  , gender(0)
  , channelStatus(0)
{
  m_type = ChannelType;
}


ChannelNotice::ChannelNotice(quint16 type, PacketReader *reader)
  : Notice(type, reader)
  , gender(0)
  , channelStatus(0)
{
  if (m_direction == Server2Client)
    channelId = reader->sender();
  else
    channelId = reader->dest();

  gender        = reader->get<quint8>();
  channelStatus = reader->get<quint8>();
  channels      = reader->idList();
}


void ChannelNotice::write(PacketWriter *writer) const
{
  writer->put(gender);
  writer->put(channelStatus);
  writer->putId(channels);
}


/*!
 * Формирование пакета для отправки клиенту заголовка канала.
 *
 * \param channel Канал.
 * \param dest    Идентификатор получателя.
 * \param command Команда.
 */
ChannelPacket ChannelNotice::channel(ClientChannel channel, const QByteArray &dest, const QString &command)
{
  ChannelPacket packet(new ChannelNotice(channel->id(), dest, command, DateTime::utc()));
  packet->setDirection(Server2Client);
  packet->setText(channel->name());
  packet->gender        = channel->gender().raw();
  packet->channelStatus = channel->status().value();
//  packet.setData(channel->feeds().headers(0));

  if (channel->type() == SimpleID::ChannelId)
    packet->channels = channel->channels().all();

  return packet;
}


ChannelPacket ChannelNotice::info(ClientChannel channel, qint64 date)
{
  ChannelPacket packet(new ChannelNotice(channel->id(), channel->id(), CHANNELS_INFO_CMD, date ? date : DateTime::utc()));
  packet->setDirection(Server2Client);
  packet->setText(channel->name());
  packet->gender        = channel->gender().raw();
  packet->channelStatus = channel->status().value();
  return packet;
}


/*!
 * Формирование пакета "info" для запроса информации о каналах.
 *
 * \param user     Идентификатор пользователя отправителя.
 * \param channels Список идентификаторов каналов, о которых необходима информация.
 */
ChannelPacket ChannelNotice::info(const QByteArray &user, const QList<QByteArray> &channels)
{
  ChannelPacket packet(new ChannelNotice(user, user, CHANNELS_INFO_CMD, DateTime::utc()));
  packet->channels = channels;
  return packet;
}


/*!
 * Базовый пакет ответа за запрос клиента.
 */
ChannelPacket ChannelNotice::reply(const ChannelNotice &source, int status)
{
  ChannelPacket packet(new ChannelNotice(source.dest(), source.sender(), source.command()));
  packet->setDirection(Server2Client);
  packet->setText(source.text());
  packet->setStatus(status);
  return packet;
}


/*!
 * Базовая функция формирования запроса к серверу.
 */
ChannelPacket ChannelNotice::request(const QByteArray &user, const QByteArray &channel, const QString &command, const QString &text)
{
  ChannelPacket packet(new ChannelNotice(user, channel, command));
  packet->setText(text);
  return packet;
}


/*!
 * Отправка обновлённой информации о себе.
 */
ChannelPacket ChannelNotice::update(ClientChannel channel)
{
  ChannelPacket packet(new ChannelNotice(channel->id(), channel->id(), CHANNELS_UPDATE_CMD, DateTime::utc()));
  packet->setText(channel->name());
  packet->gender        = channel->gender().raw();
  packet->channelStatus = channel->status().value();
  return packet;
}
