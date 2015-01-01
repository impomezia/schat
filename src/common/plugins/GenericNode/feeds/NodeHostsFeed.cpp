/* $Id: NodeHostsFeed.cpp 3735 2013-07-07 23:38:33Z IMPOMEZIA $
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
#include "cores/Core.h"
#include "DateTime.h"
#include "events.h"
#include "feeds/NodeHostsFeed.h"
#include "net/packets/ChannelNotice.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "ServerChannel.h"
#include "sglobal.h"
#include "tools/Ver.h"

NodeHostsFeed::NodeHostsFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  m_header.acl().setMask(0400);
}


NodeHostsFeed::NodeHostsFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  m_header.acl().setMask(0400);
}


FeedReply NodeHostsFeed::del(const QString &path, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  const QByteArray id = SimpleID::decode(path);
  if (SimpleID::typeOf(id) != SimpleID::HostId)
    return Notice::BadRequest;

  ServerChannel *user = static_cast<ServerChannel *>(head().channel());
  if (!user->hosts()->all().contains(id))
    return Notice::NotFound;

  user->hosts()->unlink(id);

  return FeedReply(Notice::OK, DateTime::utc());
}


FeedReply NodeHostsFeed::post(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(path)
  Q_UNUSED(json)
  Q_UNUSED(channel)
  Q_UNUSED(blob)
  return Notice::Forbidden;
}


FeedReply NodeHostsFeed::put(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(path)
  Q_UNUSED(json)
  Q_UNUSED(channel)
  Q_UNUSED(blob)
  return Notice::Forbidden;
}


/*!
 * Получение тела фида.
 */
QVariantMap NodeHostsFeed::feed(Channel *channel) const
{
  if (head().channel()->type() != SimpleID::UserId || !can(channel, Acl::Read))
    return QVariantMap();

  QVariantMap out;

  ServerChannel *user = static_cast<ServerChannel *>(head().channel());
  const QMap<ChatId, HostInfo> &hosts = user->hosts()->all();
  foreach (const HostInfo &info, hosts) {
    QVariantMap data;
    data[LS("online")]  = info->online;
    data[LS("name")]    = info->name;
    data[LS("host")]    = info->ip;
    data[LS("version")] = Ver(info->version).toString();
    data[LS("os")]      = info->os;
    data[LS("osName")]  = info->osName;
    data[LS("tz")]      = info->tz;
    data[LS("date")]    = info->date;

    merge(LS("geo"),  data, info->geo);
    merge(LS("data"), data, info->data);
    out[info->hostId.toString()] = data;
  }

  return out;
}


Feed* NodeHostsFeedCreator::create(const QString &name) const
{
  return new NodeHostsFeed(name, DateTime::utc());
}


Feed* NodeHostsFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeHostsFeed(name, data);
}
