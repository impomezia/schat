/* $Id: Notice.cpp 3734 2013-07-07 22:07:19Z IMPOMEZIA $
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

#include "net/PacketReader.h"
#include "net/packets/Notice.h"
#include "net/PacketWriter.h"

/*!
 * Базовый конструктор.
 */
Notice::Notice(const QByteArray &sender, const QByteArray &dest, const QString &command, quint64 date, const QByteArray &id, const QVariantMap &data)
  : m_sender(sender)
  , m_dest(dest)
  , m_type(GenericType)
  , m_direction(Client2Server)
  , m_fields(0)
  , m_version(0)
  , m_status(OK)
  , m_date(date)
  , m_id(id)
  , m_command(command)
  , m_data(data)
{
  if (SimpleID::typeOf(m_id) == SimpleID::MessageId)
    m_fields |= IdField;

  if (!data.isEmpty())
    m_fields |= JSonField;
}


/*!
 * Конструктор чтения.
 */
Notice::Notice(quint16 type, PacketReader *reader)
  : m_sender(reader->sender())
  , m_dest(reader->dest())
  , m_type(type)
  , m_fields(0)
{
  m_direction = reader->get<quint8>();
  m_fields    = reader->get<quint8>();
  m_version   = reader->get<quint8>();
  m_status    = reader->get<quint16>();
  m_date      = reader->get<qint64>();

  if (m_fields & IdField)
    m_id = reader->id();

  m_command = reader->text();

  if (m_fields & JSonField) {
    m_raw = reader->get<QByteArray>();
    m_data = JSON::parse(m_raw).toMap();
  }

  if (m_fields & TextField)
    m_text = reader->text();

  if (m_fields & BlobField)
    m_blob = reader->get<QByteArray>();
}


bool Notice::isValid() const
{
  if (m_command.isEmpty())
    return false;

  if (m_fields & IdField && SimpleID::typeOf(m_id) != SimpleID::MessageId)
    return false;

  if (m_fields & JSonField && (m_data.isEmpty() && m_raw.isEmpty()))
    return false;

  if (m_fields & TextField && m_text.isEmpty())
    return false;

  if (m_fields & BlobField && m_blob.isEmpty())
    return false;

  return true;
}


/*!
 * Запись пакета.
 */
QByteArray Notice::data(QDataStream *stream, bool echo) const
{
  if (!m_data.isEmpty() || !m_raw.isEmpty())
    m_fields |= JSonField;

  if (!m_text.isEmpty())
    m_fields |= TextField;

  if (!m_blob.isEmpty())
    m_fields |= BlobField;

  PacketWriter writer(stream, Protocol::NoticePacket, m_sender, m_dest, echo);
  writer.put(m_type);
  writer.put(m_direction);
  writer.put(m_fields);
  writer.put(m_version);
  writer.put(m_status);
  writer.put(m_date);

  if (m_fields & IdField)
    writer.putId(m_id);

  writer.put(m_command);

  if (m_fields & JSonField) {
    if (m_raw.isEmpty())
      writer.put(QVariant(m_data));
    else
      writer.put(m_raw);
  }

  if (m_fields & TextField)
    writer.put(m_text);

  if (m_fields & BlobField)
    writer.put(m_blob);

  write(&writer);

  return writer.data();
}


QString Notice::status(int status)
{
  switch (status) {
    case OK:
      return QObject::tr("OK");

    case Found:
      return QObject::tr("Found");

    case NotModified:
      return QObject::tr("Not Modified");

    case BadRequest:
      return QObject::tr("Bad Request");

    case Unauthorized:
      return QObject::tr("Unauthorized");

    case Forbidden:
      return QObject::tr("Forbidden");

    case NotFound:
      return QObject::tr("Not Found");

    case ObjectAlreadyExists:
      return QObject::tr("Object Already Exists");

    case ObjectNotExists:
      return QObject::tr("Object Not Exists");

    case NickAlreadyUse:
      return QObject::tr("Nick Already In Use");

    case ChannelOffline:
      return QObject::tr("Channel Offline");

    case Conflict:
      return QObject::tr("Conflict");

    case InternalError:
      return QObject::tr("Internal Error");

    case NotImplemented:
      return QObject::tr("Not Implemented");

    case BadGateway:
      return QObject::tr("Bad Gateway");

    case ServiceUnavailable:
      return QObject::tr("Service Unavailable");

    case GatewayTimeout:
      return QObject::tr("Gateway Timeout");

    default:
      return QObject::tr("Unknown");
  }
}
