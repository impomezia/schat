/* $Id: NodeUsersFeed.cpp 3735 2013-07-07 23:38:33Z IMPOMEZIA $
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

#include "Ch.h"
#include "Channel.h"
#include "DataBase.h"
#include "DateTime.h"
#include "feeds/NodeUsersFeed.h"
#include "feeds/ServerFeed.h"
#include "feeds/UsersFeed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "Storage.h"

NodeUsersFeed::NodeUsersFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  init();
}


NodeUsersFeed::NodeUsersFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  init();
}


FeedReply NodeUsersFeed::del(const QString &path, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (!can(channel, Acl::Edit | Acl::SpecialEdit))
    return Notice::Forbidden;

  if (path.size() == 34) {
    if (SimpleID::typeOf(SimpleID::decode(path)) == SimpleID::UserId) {
      ServerChannel *channel = static_cast<ServerChannel*>(head().channel());
      m_data[USERS_FEED_COUNT_KEY]   = channel->channels().all(SimpleID::UserId).size();
      m_data[USERS_FEED_OFFLINE_KEY] = channel->offline().size();

      dump();
      return FeedReply(Notice::OK, DateTime::utc());
    }
  }

  return Notice::NotModified;
}


FeedReply NodeUsersFeed::post(const QString &path, const QVariantMap &json, Channel *user, const QByteArray &blob)
{
  Q_UNUSED(json)
  Q_UNUSED(blob)

  if (!can(user, Acl::Edit | Acl::SpecialEdit))
    return Notice::Forbidden;

  if (path.size() == 34) {
    if (SimpleID::typeOf(SimpleID::decode(path)) == SimpleID::UserId) {
      ServerChannel *channel = static_cast<ServerChannel*>(head().channel());

      const int count   = channel->channels().all(SimpleID::UserId).size();
      const qint64 date = DateTime::utc();

      const int peak = m_data.value(USERS_FEED_PEAK_KEY).toMap().value(USERS_FEED_COUNT_KEY).toInt();
      if (count >= peak)
        setPeak(count, date);

      m_data[USERS_FEED_COUNT_KEY]   = count;
      m_data[USERS_FEED_OFFLINE_KEY] = channel->offline().size();

      dump();
      return FeedReply(Notice::OK, date);
    }
  }

  return Notice::NotModified;
}


void NodeUsersFeed::setChannel(Channel *channel)
{
  Feed::setChannel(channel);
  restore();

  if (channel->type() != SimpleID::ServerId)
    return;

  if (!m_data.contains(USERS_FEED_PEAK_KEY)) {
    const QVariantMap peak = Storage::value(STORAGE_PEAK_USERS).toMap();
    if (peak.isEmpty())
      setPeak(0, head().date());
    else
      m_data[USERS_FEED_PEAK_KEY] = peak;
  }
}


/*!
 * Возвращает \b true если этот канал поддерживает список офлайн пользователей.
 */
bool NodeUsersFeed::isSupportOfflineUsers() const
{
  return (head().channel()->type() == SimpleID::ChannelId && Ch::server()->feed(FEED_NAME_SERVER)->data().value(SERVER_FEED_OFFLINE_KEY, true).toBool());
}


/*!
 * Запись дампа списка пользователей в базу данных.
 */
void NodeUsersFeed::dump() const
{
  if (!isSupportOfflineUsers())
    return;

  ServerChannel *channel = static_cast<ServerChannel*>(head().channel());
  DataBase::setValue(SimpleID::encode(channel->id()) + LS("/users"), channel->channels().join() + channel->offline().join());
}


void NodeUsersFeed::init()
{
  m_header.acl().setMask(0444);
  m_data[USERS_FEED_COUNT_KEY] = 0;
}


/*!
 * Восстановление списка пользователей из базы данных.
 */
void NodeUsersFeed::restore()
{
  if (!isSupportOfflineUsers())
    return;

  ServerChannel *channel = static_cast<ServerChannel*>(head().channel());
  const QByteArray data  = DataBase::value(SimpleID::encode(channel->id()) + LS("/users")).toByteArray();

  channel->offline().restore(data);
  m_data[USERS_FEED_COUNT_KEY]   = 0;
  m_data[USERS_FEED_OFFLINE_KEY] = channel->offline().size();
}


void NodeUsersFeed::setPeak(int count, qint64 date)
{
  QVariantMap peak;
  peak[USERS_FEED_COUNT_KEY]  = count;
  peak[USERS_FEED_DATE_KEY]   = date;
  m_data[USERS_FEED_PEAK_KEY] = peak;
}


Feed* NodeUsersFeedCreator::create(const QString &name) const
{
  return new NodeUsersFeed(name, DateTime::utc());
}


Feed* NodeUsersFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeUsersFeed(name, data);
}
