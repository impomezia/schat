/* $Id: TransportWriter.cpp 3715 2013-06-26 23:10:22Z IMPOMEZIA $
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

#include "net/Protocol.h"
#include "net/TransportWriter.h"

/*!
 * Создание транспортного пакета.
 *
 * \param stream     output stream.
 * \param packets    raw virtual packets.
 * \param sequence   packet sequence.
 * \param timestamp  отметка времени.
 * \param options    packet options.
 * \param type       packet type.
 * \param subversion packet subversion.
 */
TransportWriter::TransportWriter(QDataStream *stream, const QList<QByteArray> &packets, quint64 sequence, qint64 timestamp, quint8 options, quint8 type, quint8 subversion)
  : m_device(stream->device()),
    m_size(16)
{
  m_device->seek(0);
  quint32 size = packets.size();
  bool huge = options & Protocol::HugePackets;

  // Расчёт размера пакета и составление карты размеров виртуальных пакетов.
  if (huge)
    m_size += (size * 4);
  else
    m_size += (size * 2);

  quint32 *map = new quint32[size];
  quint32 s = 0;
  for (quint32 i = 0; i < size; ++i) {
    s = packets.at(i).size();
    map[i] = s;
    m_size += s;
  }

  if (timestamp != 0) {
    options |= Protocol::TimeStamp;
    m_size += 8;
  }

  // Запись заголовка пакета.
  *stream << m_size << type << quint8(Protocol::V4) << subversion << options << sequence;

  if (timestamp != 0)
    *stream << timestamp;

  *stream << size;
  // Запись карты размеров виртуальных пакетов.
  if (huge) {
    for (quint32 i = 0; i < size; ++i)
      *stream << map[i];
  } else {
    for (quint32 i = 0; i < size; ++i)
      *stream << quint16(map[i]);
  }

  // Запись тел виртуальных пакетов.
  for (quint32 i = 0; i < size; ++i) {
    stream->writeRawData(packets.at(i).constData(), map[i]);
  }

  delete [] map;
}
