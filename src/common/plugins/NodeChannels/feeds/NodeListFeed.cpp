/* $Id: NodeListFeed.cpp 3735 2013-07-07 23:38:33Z IMPOMEZIA $
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
#include "DateTime.h"
#include "feeds/FeedsCore.h"
#include "feeds/ListFeed.h"
#include "feeds/NodeListFeed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "NodeChannelIndex.h"
#include "NodeChannelsPlugin_p.h"
#include "Normalize.h"
#include "sglobal.h"

NodeListFeed::NodeListFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  init();
}


NodeListFeed::NodeListFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  init();
}


/*!
 * Переопределение запроса \b delete.
 */
FeedReply NodeListFeed::del(const QString &path, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(path)
  Q_UNUSED(channel)
  Q_UNUSED(blob)
  return Notice::Forbidden;
}


FeedReply NodeListFeed::get(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob) const
{
  Q_UNUSED(blob)

  if (path.isEmpty())
    return Notice::BadRequest;

  if (path == LIST_FEED_ID_KEY)
    return id(json.value(FEED_KEY_VALUE).toString());

  return Feed::get(path, json, channel);
}


/*!
 * Переопределение запроса \b post.
 */
FeedReply NodeListFeed::post(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(path)
  Q_UNUSED(json)
  Q_UNUSED(channel)
  Q_UNUSED(blob)
  return Notice::Forbidden;
}


/*!
 * Переопределение запроса \b put.
 */
FeedReply NodeListFeed::put(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(path)
  Q_UNUSED(json)
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  if (path == LIST_FEED_CHANNELS_KEY && Ch::server() == channel) {
    const QList<ChannelIndexData> &list = NodeChannelsImpl::index()->list();
    QVariantList channels;

    foreach (const ChannelIndexData &data, list)
      channels.push_back(data.toList());

    m_data[LS("channels")] = channels;
    m_header.setDate(NodeChannelsImpl::index()->date());
    return FeedReply(Notice::OK, DateTime::utc());
  }

  return Notice::Forbidden;
}


FeedReply NodeListFeed::id(const QString &name) const
{
  if (!Channel::isValidName(name))
    return Notice::BadRequest;

  FeedReply reply(Notice::NotFound);
  const QByteArray normalized = Normalize::toId(SimpleID::ChannelId, name);
  ChatChannel channel         = Ch::channel(normalized);

  if (!channel) {
    const QByteArray id = Ch::makeId(normalized);
    channel = Ch::channel(id);
    reply.json[FEED_KEY_VALUE] = SimpleID::encode(id);
  }

  if (channel) {
    reply.status               = Notice::OK;
    reply.json[FEED_KEY_VALUE] = SimpleID::encode(channel->id());
    reply.date                 = head().date();
  }

  return reply;
}


void NodeListFeed::init()
{
  m_header.acl().setMask(0444);
  m_data[LIST_FEED_FORMAT_KEY] = QVariantList() << LS("id") << LS("name") << LS("count") << LS("title") << LS("options");

  FeedsCore::sub(FEED_NAME_LIST);
}


Feed* NodeListFeedCreator::create(const QString &name) const
{
  return new NodeListFeed(name, DateTime::utc());
}


Feed* NodeListFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeListFeed(name, data);
}
