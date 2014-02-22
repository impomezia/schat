/* $Id: NodeChannelFeed.cpp 3735 2013-07-07 23:38:33Z IMPOMEZIA $
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
#include "cores/Core.h"
#include "DataBase.h"
#include "DateTime.h"
#include "events.h"
#include "feeds/ChannelFeed.h"
#include "feeds/NodeChannelFeed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "ServerChannel.h"

NodeChannelFeed::NodeChannelFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  m_header.acl().setMask(0744);
}


NodeChannelFeed::NodeChannelFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  m_header.acl().setMask(0744);
}



/*!
 * Для записи фида в обычных каналах используется проверка на владельца фида acl.
 */
bool NodeChannelFeed::can(Channel *channel, int acl) const
{
  if (head().channel()->type() == SimpleID::ChannelId && acl == Acl::Write) {
    FeedPtr feed = m_header.channel()->feed(FEED_NAME_ACL, false, false);
    if (feed)
      return feed->can(channel, Acl::Edit);
  }

  return Feed::can(channel, acl);
}


FeedReply NodeChannelFeed::del(const QString &path, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (isReservedKey(path) || path == FEED_WILDCARD_ASTERISK)
    return Notice::BadRequest;

  return Feed::del(path, channel);
}


FeedReply NodeChannelFeed::post(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (isReservedKey(path))
    return update(path, json, channel);

  return Feed::post(path, json, channel);
}


FeedReply NodeChannelFeed::put(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (isReservedKey(path))
    return update(path, json, channel);

  return Feed::put(path, json, channel);
}


void NodeChannelFeed::setChannel(Channel *channel)
{
  Feed::setChannel(channel);

  m_data[CHANNEL_FEED_NAME_KEY]   = channel->name();
  m_data[CHANNEL_FEED_GENDER_KEY] = channel->gender().raw();
  m_data[CHANNEL_FEED_STATUS_KEY] = channel->status().value();
  m_data[CHANNEL_FEED_TYPE_KEY]   = SimpleID::typeOf(channel->id());
}


/*!
 * Возвращает \b true если имя ключа является зарезервированным для полей канала.
 */
bool NodeChannelFeed::isReservedKey(const QString &key)
{
  return (key == CHANNEL_FEED_GENDER_KEY || key == CHANNEL_FEED_NAME_KEY || key == CHANNEL_FEED_STATUS_KEY || key == CHANNEL_FEED_TYPE_KEY);
}


bool NodeChannelFeed::isValidName(const QString &name) const
{
  if (SimpleID::typeOf(head().channel()->id()) == SimpleID::ServerId && name.isEmpty())
    return true;

  return Channel::isValidName(name);
}


/*!
 * Обновление канала.
 */
FeedReply NodeChannelFeed::update(const QString &key, const QVariantMap &json, Channel *user)
{
  Q_UNUSED(user)

  if (!json.contains(FEED_KEY_VALUE) || key == CHANNEL_FEED_TYPE_KEY)
    return Notice::BadRequest;

  ChatChannel channel = Ch::channel(head().channel()->id(), head().channel()->type());
  if (!channel)
    return Notice::InternalError;

  const QVariant& value = json[FEED_KEY_VALUE];
  const qint64 date     = DateTime::utc();

  if (key == CHANNEL_FEED_NAME_KEY) {
    const QString name = value.toString();
    if (!isValidName(name))
      return Notice::BadRequest;

    if (channel->name() == name)
      return Notice::NotModified;

    const int status = Ch::rename(channel, name);
    if (status != Notice::OK)
      return status;

    m_data[CHANNEL_FEED_NAME_KEY] = channel->name();
  }
  else if (key == CHANNEL_FEED_GENDER_KEY) {
    const int gender = value.toInt();
    if (channel->gender().raw() == gender)
      return Notice::NotModified;

    channel->gender() = gender;
    m_data[CHANNEL_FEED_GENDER_KEY] = channel->gender().raw();
  }
  else if (key == CHANNEL_FEED_STATUS_KEY) {
    const int status = value.toInt();
    if (status == Status::Offline) {
      Core::i()->send(QList<quint64>() << Core::socket(), QByteArray(), NewPacketsEvent::KillSocketOption);
      return Notice::NotModified;
    }

    if (channel->status().value() == status)
      return Notice::NotModified;

    channel->status() = status;
    m_data[CHANNEL_FEED_STATUS_KEY] = channel->status().value();
  }
  else
    return Notice::NotModified;

  channel->setDate(date);
  DataBase::add(channel);

  return FeedReply(Notice::OK, date);
}


Feed* NodeChannelFeedCreator::create(const QString &name) const
{
  return new NodeChannelFeed(name, DateTime::utc());
}


Feed* NodeChannelFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeChannelFeed(name, data);
}
