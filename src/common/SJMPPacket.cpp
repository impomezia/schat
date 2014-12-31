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

#include <QDateTime>

#include "SJMPPacket.h"
#include "JSON.h"
#include "ShortID.h"

SJMPPacket::SJMPPacket()
  : m_type(Request)
  , m_version(1)
  , m_status(200)
  , m_date(0)
{
  m_id = ShortID::generate();
}


SJMPPacket::SJMPPacket(const QByteArray &packet)
  : m_type(Unknown)
  , m_version(1)
  , m_status(200)
  , m_date(0)
{
  deserialize(packet);
}


bool SJMPPacket::deserialize(const QByteArray &packet)
{
  if (packet.isEmpty() || !packet.startsWith('['))
    return false;

  const QVariantList list = JSON::parse(packet).toList();
  if (list.size() < 6)
    return false;

  QByteArray data = list.at(0).toByteArray();
  if (data.isEmpty())
    return false;

  m_type = static_cast<Type>(data.at(0));
  if (m_type != Request && m_type != Reply && m_type != Modified && m_type != Added && m_type != Deleted) {
    m_type = Unknown;
    return false;
  }

  QList<QByteArray> header = data.remove(0, 1).split('/');

  if (m_type == Request) {
    m_method = header.at(0);

    if (m_method.isEmpty())
      m_method = QLatin1String("get");
    else if (m_method == QLatin1String("+"))
      m_method = QLatin1String("post");
    else if (m_method == QLatin1String("="))
      m_method = QLatin1String("put");
    else if (m_method == QLatin1String("-"))
      m_method = QLatin1String("delete");
  }

  if (m_type == Reply) {
    if (header.at(0).isEmpty())
      m_status = 200;
    else if ((m_status = header.at(0).toInt()) == 0)
      return false;
  }

  if (header.size() > 1) {
    m_version = header.at(1).toInt();

    if (m_version < 1)
      m_version = 1;
  }

  m_resource = list.at(1).toString();
  m_id       = list.at(2).toString();

  if (m_id.size() < 4)
    return false;

  m_date    = list.at(3).toLongLong();
  m_headers = list.at(4).toMap();
  m_body    = list.at(5);

  return true;
}


QByteArray SJMPPacket::serialize() const
{
  if (m_type == Unknown)
    return QByteArray();

  QVariantList list;
  list.reserve(6);

  QString header(m_type);
  if (m_type == Request) {
    if (m_method == QLatin1String("post"))
      header += '+';
    else if (m_method == QLatin1String("put"))
      header += '=';
    else if (m_method == QLatin1String("delete"))
      header += '-';
    else if (m_method != QLatin1String("get"))
      header += m_method;
  }

  if (m_type == Reply && m_status != 200)
    header += QString::number(m_status);

  if (m_version > 1)
    header += QLatin1Char('/') + QString::number(m_version);

  list.append(header);
  list.append(m_resource);
  list.append(m_id);
  list.append(m_date);
  list.append(m_headers);
  list.append(m_body);

  return JSON::generate(list);
}


SJMPPacket &SJMPPacket::setDate(const QDateTime &dateTime)
{
# if QT_VERSION >= 0x040700
  m_date = dateTime.toMSecsSinceEpoch();
# else
  m_date = dateTime.toTime_t();
  m_date *= 1000;
  m_date += dateTime.time().msec();
# endif

  return *this;
}


QDebug operator<<(QDebug dbg, const SJMPPacket &packet)
{
  dbg.nospace() << "SJMPPacket(" << packet.serialize() << ")";
  return dbg.maybeSpace();
}
