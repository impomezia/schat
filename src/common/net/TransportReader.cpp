/* $Id: TransportReader.cpp 3759 2013-07-25 02:51:09Z IMPOMEZIA $
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
#include "net/TransportReader.h"

/*!
 * \param size   Размер данных.
 * \param stream Поток для чтения данных.
 */
TransportReader::TransportReader(qint32 size, QDataStream *stream)
  : m_stream(stream)
  , m_timestamp(0)
  , m_available(size)
{
}


/*!
 * Чтение заголовка транспортного пакета.
 * Коды ошибок:
 *  - -1 Неизвестный тип пакета.
 *  - -2 Версия пакета не равна Protocol::V4.
 *  - -3 Размер тела пакета полученных из заголовка не совпадает с фактическим размером пакета.
 *
 * \return Тип транспортного пакета или код ошибки.
 */
int TransportReader::readHeader()
{
  *m_stream >> m_type;
  m_available--;
  if (m_type != Protocol::GenericTransport)
    return -1;

  *m_stream >> m_version >> m_subversion;
  m_available -= 2;
  if (m_version != Protocol::V4)
    return -2;

  *m_stream >> m_options >> m_sequence;
  m_available -= 9;

  if (m_options & Protocol::TimeStamp) {
    *m_stream >> m_timestamp;
    ((short *) &m_timestamp)[3] = 0; /// \since 2.1.0 2 старших байт отметки времени зарезервированы для будущего использования.
    m_available -= 8;
  }

  if (!createMap())
    return -3;

  return m_type;
}


/*!
 * Чтение одного виртуального пакета.
 */
QByteArray TransportReader::readOne()
{
  if (m_sizes.size()) {
    m_available -= m_sizes.at(0);
    return m_stream->device()->read(m_sizes.takeFirst());
  }

  return QByteArray();
}


/*!
 * Чтение транспортного пакета и получение списка тел виртуальных пакетов.
 * \return Список тел виртуальных пакетов.
 */
QList<QByteArray> TransportReader::read()
{
  QList<QByteArray> out;
  for (int i = 0; i < m_sizes.size(); ++i) {
    out.append(m_stream->device()->read(m_sizes.at(i)));
  }
  return out;
}


/*!
 * Пропускает все оставшиеся данные в пакете, после этой операции можно читать следующий
 * транспортный пакет.
 */
void TransportReader::skipAll()
{
  m_stream->device()->read(m_available);
  m_available = 0;
}


bool TransportReader::createMap()
{
  quint32 count = 0;
  *m_stream >> count;
  m_available -= 4;
  if (m_available < 0 || count > Protocol::MaxVirtualPackets)
    return false;

  qint32 checkSize = 0;

  if (m_options & Protocol::HugePackets) {
    m_options = m_options ^ Protocol::HugePackets;
    m_available -= (count * 4);
    if (m_available < 0)
      return false;

    quint32 size = 0;
    for (quint32 i = 0; i < count; ++i) {
      *m_stream >> size;
      checkSize += size;
      m_sizes.append(size);
    }
  }
  else {
    m_available -= (count * 2);
    if (m_available < 0)
      return false;

    quint16 size = 0;
    for (quint32 i = 0; i < count; ++i) {
      *m_stream >> size;
      checkSize += size;
      m_sizes.append(size);
    }
  }

  return checkSize == m_available;
}
