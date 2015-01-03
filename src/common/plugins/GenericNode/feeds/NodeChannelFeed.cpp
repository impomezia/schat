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
#include "sglobal.h"
#include "SJMPPacket.h"

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
  m_data[CHANNEL_FEED_STATUS_KEY] = channel->status();
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
FeedReply NodeChannelFeed::update(const QString &path, const QVariantMap &json, Channel *user)
{
  Q_UNUSED(user)

  if (!json.contains(FEED_KEY_VALUE) || path == CHANNEL_FEED_TYPE_KEY)
    return Notice::BadRequest;

  ChatChannel channel = Ch::channel(head().channel()->id(), head().channel()->type());
  if (!channel)
    return Notice::InternalError;

  QVariantMap body;
  body.insert(path == CHANNEL_FEED_NAME_KEY ? LS("nick") : path, json[LS("value")]);

  SJMPPacket packet;
  packet.setMethod(LS("put"));
  packet.setResource(LS("v4/user/") + head().channel()->nativeId());
  packet.setBody(body);

  const SJMPPacket reply = Core::sendSync(packet);

  if (reply.status() != 200)
    return Notice::NotModified;

  head().channel()->setData(LS("profile"), reply.body());

  if (path == CHANNEL_FEED_NAME_KEY) {
    channel->setName(reply.body().toMap().value(LS("nick")).toString());

    Ch::rename(channel, channel->name());
    m_data[CHANNEL_FEED_NAME_KEY] = channel->name();
  }
  else if (path == CHANNEL_FEED_GENDER_KEY) {
    channel->gender().set(reply.body().toMap().value(CHANNEL_FEED_GENDER_KEY).toString(), reply.body().toMap().value(LS("color")).toString());
    m_data[CHANNEL_FEED_GENDER_KEY] = channel->gender().raw();
  }
  else if (path == CHANNEL_FEED_STATUS_KEY) {
    channel->setStatus(reply.body().toMap().value(CHANNEL_FEED_STATUS_KEY).toInt());
    m_data[CHANNEL_FEED_STATUS_KEY] = channel->status();
  }

  channel->setDate(reply.date());
  DataBase::add(channel);

  return FeedReply(Notice::OK, reply.date());
}


Feed* NodeChannelFeedCreator::create(const QString &name) const
{
  return new NodeChannelFeed(name, DateTime::utc());
}


Feed* NodeChannelFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeChannelFeed(name, data);
}
