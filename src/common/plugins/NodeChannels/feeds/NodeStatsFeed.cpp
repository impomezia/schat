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

#include "Ch.h"
#include "DateTime.h"
#include "feeds/FeedsCore.h"
#include "feeds/InfoFeed.h"
#include "feeds/NodeStatsFeed.h"
#include "feeds/StatsFeed.h"
#include "net/packets/Notice.h"
#include "ServerChannel.h"

NodeStatsFeed::NodeStatsFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  init();
}


NodeStatsFeed::NodeStatsFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  init();
}


FeedReply NodeStatsFeed::del(const QString &path, Channel *user, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (!can(user, Acl::Edit | Acl::SpecialEdit))
    return Notice::Forbidden;

  if (path.size() == ChatId::kEncodedSize) {
    ServerChannel *channel = static_cast<ServerChannel*>(head().channel());
    if (!channel || channel->type() == ChatId::UserId)
      return Notice::BadRequest;

    ChatId id(path);
    if (id.isNull())
      return Notice::BadRequest;

    if (id.type() == ChatId::UserId) {
      QVariantList users = m_data.value(STATS_FEED_USERS_KEY).toList();
      if (users.size() != 3)
        users = QVariantList() << 0 << 0 << 0;

      users[0] = channel->channels().all(ChatId::UserId).size();
      m_data.insert(STATS_FEED_USERS_KEY, users);
      return FeedReply(Notice::OK, DateTime::utc());
    }
  }

  return Notice::NotImplemented;
}


FeedReply NodeStatsFeed::post(const QString &path, const QVariantMap &json, Channel *user, const QByteArray &blob)
{
  Q_UNUSED(json)
  Q_UNUSED(blob)

  if (!can(user, Acl::Edit | Acl::SpecialEdit))
    return Notice::Forbidden;

  if (path.size() == ChatId::kEncodedSize) {
    ServerChannel *channel = static_cast<ServerChannel*>(head().channel());
    if (!channel || channel->type() == ChatId::UserId)
      return Notice::BadRequest;

    ChatId id(path);
    if (id.isNull())
      return Notice::BadRequest;

    if (id.type() == ChatId::UserId) {
      const int count    = channel->channels().all(ChatId::UserId).size();
      const qint64 date  = DateTime::utc();
      QVariantList users = m_data.value(STATS_FEED_USERS_KEY).toList();
      if (users.size() != 3)
        users = QVariantList() << 0 << 0 << 0;

      const int peak = users[1].toInt();
      if (count >= peak) {
        users[1] = count;
        users[2] = date;
      }

      users[0] = count;
      m_data.insert(STATS_FEED_USERS_KEY, users);
      return FeedReply(Notice::OK, date);
    }
  }

  return Notice::NotImplemented;
}


FeedReply NodeStatsFeed::put(const QString &path, const QVariantMap &json, Channel *user, const QByteArray &blob)
{
  Q_UNUSED(json)
  Q_UNUSED(blob)

  if (!can(user, Acl::Edit | Acl::SpecialEdit))
    return Notice::Forbidden;

  if (path == STATS_FEED_ROOMS_KEY)
    return FeedReply(Notice::OK, rooms());

  return Notice::NotImplemented;
}


void NodeStatsFeed::setChannel(Channel *channel)
{
  Feed::setChannel(channel);

  if ((channel->type() == ChatId::ServerId || channel->type() == ChatId::ChannelId) && !m_data.contains(STATS_FEED_USERS_KEY)) {
    m_data.insert(STATS_FEED_USERS_KEY, QVariantList() << 0 << 0 << 0); /// [current, peak, peak date]
  }

  if (channel->type() == ChatId::ServerId && !m_data.contains(STATS_FEED_ROOMS_KEY)) {
    m_data.insert(STATS_FEED_ROOMS_KEY, QVariantList() << 0 << 0 << 0 << 0 << 0); /// [current total, current visible, current hidden, peak, peak date]
  }
}


void NodeStatsFeed::init()
{
  m_header.acl().setMask(0444);

  FeedsCore::sub(FEED_NAME_STATS);
}


qint64 NodeStatsFeed::rooms()
{
  ServerChannel *channel           = static_cast<ServerChannel*>(head().channel());
  const QList<QByteArray> channels = channel->channels().all(ChatId::ChannelId);
  const qint64 date  = DateTime::utc();
  QVariantList rooms = m_data.value(STATS_FEED_ROOMS_KEY).toList();
  if (rooms.size() != 5)
    rooms = QVariantList() << 0 << 0 << 0 << 0 << 0;

  int visible = 0;
  int hidden  = 0;

  foreach (const QByteArray &id, channels) {
    ChatChannel room = Ch::channel(id, ChatId::ChannelId, false);
    if (!room)
      continue;

    if (room->permanent()) {
      ++visible;
      continue;
    }

    if (room->channels().size()) {
      FeedPtr feed = room->feed(FEED_NAME_INFO, false);
      if (!feed)
        continue;

      const int visibility = feed->data().value(INFO_FEED_VISIBILITY_KEY, 0).toInt();
      if (visibility < 0)
        ++hidden;
      else
        ++visible;
    }
  }

  const int count = visible + hidden;

  rooms[0] = count;
  rooms[1] = visible;
  rooms[2] = hidden;

  const int peak = rooms[3].toInt();
  if (count >= peak) {
    rooms[3] = count;
    rooms[4] = date;
  }

  m_data.insert(STATS_FEED_ROOMS_KEY, rooms);
  return date;
}


Feed* NodeStatsFeedCreator::create(const QString &name) const
{
  return new NodeStatsFeed(name, DateTime::utc());
}


Feed* NodeStatsFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeStatsFeed(name, data);
}
