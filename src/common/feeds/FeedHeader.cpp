/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

#include "Channel.h"
#include "DateTime.h"
#include "feeds/FeedHeader.h"
#include "feeds/FeedStrings.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "sglobal.h"

/*!
 * Конструктор создающий пустой объект.
 */
FeedHeader::FeedHeader()
  : m_channel(0)
  , m_key(0)
{
  setRev(0);
}


bool FeedHeader::isValid() const
{
  if (m_name.isEmpty())
    return false;

  if (!m_channel)
    return false;

  return true;
}


int FeedHeader::del(const QString &path)
{
  if (path.startsWith(LS("owner/")) || path.startsWith(LS("other/"))) {
    const QByteArray id = SimpleID::decode(path.mid(6));
    if (SimpleID::typeOf(id) != SimpleID::UserId)
      return Notice::BadRequest;

    if (!m_acl.owners().contains(id) && !m_acl.others().contains(id))
      return Notice::NotFound;

    m_acl.remove(id);
    m_acl.removeOther(id);
    return Notice::OK;
  }

  return Notice::NotFound;
}


int FeedHeader::post(const QString &path, const QVariant &value, Channel *user)
{
  if (path == LS("owner")) {
    const QByteArray id = SimpleID::decode(value.toString());
    if (SimpleID::typeOf(id) != SimpleID::UserId)
      return Notice::BadRequest;

    if (m_acl.owners().contains(id))
      return Notice::NotModified;

    m_acl.removeOther(id);
    m_acl.add(id);
    return Notice::OK;
  }
  else if (path.startsWith(LS("other/"))) {
    const QByteArray id = SimpleID::decode(path.mid(6));

    if (user && user->id() == id && (m_acl.match(user) & (Acl::Edit | Acl::SpecialEdit)))
      return Notice::BadRequest;

    if (!m_acl.add(id, value.toInt()))
      return Notice::BadRequest;

    m_acl.remove(id);
    return Notice::OK;
  }

  return Notice::BadRequest;
}


int FeedHeader::put(const QString &path, const QVariant &value)
{
  if (path == LS("mask")) {
    int mask = value.toInt();
    if (mask < 0 || mask > 511)
      return Notice::BadRequest;

    m_acl.setMask(mask);
    return Notice::OK;
  }

  return Notice::NotFound;
}


QVariantMap FeedHeader::f(Channel *channel) const
{
  QVariantMap json;
  if (!(acl().match(channel) & Acl::Read))
    return json;

  json[name()] = date();
  return json;
}


QVariantMap FeedHeader::get(Channel *channel) const
{
  QVariantMap data = m_data;
  if (!m_acl.get(data, channel))
    return QVariantMap();

  return data;
}


QVariantMap FeedHeader::save() const
{
  QVariantMap data = m_data;
  m_acl.save(data);
  return data;
}


/*!
 * Установка данных заголовка фида на основе JSON данных.
 */
void FeedHeader::setData(const QVariantMap &data)
{
  m_data = data;

  if (data.contains(FEED_NAME_ACL)) {
    m_acl.load(data.value(FEED_NAME_ACL).toMap());
    m_data.remove(FEED_NAME_ACL);
  }
  else
    m_acl.load(data);

  setRev(data.value(LS("rev")).toLongLong());
  m_data[LS("date")] = data.value(LS("date")).toLongLong();
}


void FeedHeader::setDate(qint64 date)
{
  if (!date)
    date = DateTime::utc();

  m_data[LS("date")] = date;
}
