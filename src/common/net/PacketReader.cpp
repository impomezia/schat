/* $Id: PacketReader.cpp 3610 2013-03-31 01:46:52Z IMPOMEZIA $
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

#include "JSON.h"
#include "net/PacketReader.h"
#include "net/Protocol.h"

PacketReader::PacketReader(QDataStream *stream)
  : m_stream(stream),
    m_device(stream->device())
{
  m_device->seek(0);
  quint8 subtype;
  *stream >> m_type >> subtype >> m_option;

  if (is(Protocol::SenderField))
    m_sender = id();

  if (is(Protocol::ChannelField))
    m_channel = id();

  if (is(Protocol::DestinationField)) {
    get<quint32>();
    m_dest = id();
  }
}


QVariant PacketReader::json() const
{
  QByteArray data;
  *m_stream >> data;
  return JSON::parse(data);
}
