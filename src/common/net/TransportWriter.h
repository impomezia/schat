/* $Id: TransportWriter.h 3714 2013-06-26 00:57:48Z IMPOMEZIA $
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

#ifndef TRANSPORTWRITER_H_
#define TRANSPORTWRITER_H_

#include <QDataStream>

#include "net/Protocol.h"

/*!
 * Класс, выполняющий запись транспортного пакета.
 */
class TransportWriter
{
public:
  TransportWriter(QDataStream *stream, const QList<QByteArray> &packets, quint64 sequence, qint64 timestamp = 0, quint8 options = 0x0, quint8 type = Protocol::GenericTransport, quint8 subversion = Protocol::V4_1);
  inline QByteArray data() const
  {
    m_device->seek(0);
    return m_device->peek(m_size + 4);
  }

private:
  QIODevice *m_device; ///< output device.
  quint32 m_size;      ///< size of packet body.
};

#endif /* TRANSPORTWRITER_H_ */
