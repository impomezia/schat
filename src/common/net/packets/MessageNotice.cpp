/* $Id: MessageNotice.cpp 3718 2013-06-29 23:12:22Z IMPOMEZIA $
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
#include "JSON.h"
#include "net/packets/MessageNotice.h"
#include "net/SimpleID.h"
#include "sglobal.h"

MessageNotice::MessageNotice()
  : Notice(QByteArray(), QByteArray(), LS("m"))
  , mdate(0)
{
  m_type = MessageType;
}


MessageNotice::MessageNotice(const MessageRecord &record, bool parse)
  : Notice(record.senderId, record.destId, record.command, record.date, record.messageId)
  , mdate(0)
{
  m_type = MessageType;
  setText(record.text);
  setStatus(record.status);
  m_raw = record.data;

  if (parse)
    m_data = JSON::parse(m_raw).toMap();

  if (SimpleID::typeOf(record.destId) == SimpleID::UserId)
    m_direction = Client2Client;

  if (m_date == 0)
    m_date = DateTime::utc();
}


MessageNotice::MessageNotice(const MessageRecordV2 &record, bool parse)
  : Notice(record.sender.toByteArray(), record.dest.toByteArray(), record.cmd, record.date, record.oid.toByteArray())
  , mdate(0)
{
  m_type = MessageType;
  setText(record.text);
  setStatus(record.status);
  m_raw = record.data;

  if (parse)
    m_data = JSON::parse(m_raw).toMap();

  if (record.dest.type() == ChatId::UserId)
    m_direction = Client2Client;

  if (m_date == 0)
    m_date = DateTime::utc();
}


MessageNotice::MessageNotice(const QByteArray &sender, const QByteArray &dest, const QString &text, quint64 date, const QByteArray &id)
  : Notice(sender, dest, LS("m"), date, id)
  , mdate(0)
{
  m_type = MessageType;
  setText(text);

  if (SimpleID::typeOf(dest) == SimpleID::UserId)
    m_direction = Client2Client;

  if (date == 0)
    m_date = DateTime::utc();
}


MessageNotice::MessageNotice(quint16 type, PacketReader *reader)
  : Notice(type, reader)
  , mdate(0)
{
}


QByteArray MessageNotice::toId() const
{
  return SimpleID::make(m_sender + dest() + QByteArray::number(m_date) + m_id, SimpleID::MessageId);
}


QList<QByteArray> MessageNotice::decode(const QStringList &ids)
{
  QList<QByteArray> out;
  QByteArray messageId;

  foreach (const QString &id, ids) {
    messageId = SimpleID::decode(id);
    if (SimpleID::typeOf(messageId) == SimpleID::MessageId)
      out.append(messageId);
  }

  return out;
}


/*!
 * Преобразование списка сообщений в кодированный идентификатор списка.
 */
QByteArray MessageNotice::toTag(const QStringList &messages)
{
  QByteArray raw;
  raw.reserve(messages.size() * SimpleID::DefaultSize);
  foreach (const QString &id, messages)
    raw.append(SimpleID::decode(id));

  return SimpleID::encode(SimpleID::make(raw, SimpleID::MessageId));
}


QStringList MessageNotice::encode(const QList<ChatId> &ids)
{
  QStringList out;
# if QT_VERSION >= 0x040700
  out.reserve(ids.size());
# endif

  foreach (const ChatId &id, ids)
    out.append(id.toString());

  return out;
}


QStringList MessageNotice::encode(const QList<QByteArray> &ids)
{
  QStringList out;
# if QT_VERSION >= 0x040700
  out.reserve(ids.size());
# endif

  foreach (const QByteArray &id, ids)
    out.append(SimpleID::encode(id));

  return out;
}
