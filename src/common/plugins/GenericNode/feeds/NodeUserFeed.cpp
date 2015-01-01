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

#include "DateTime.h"
#include "feeds/NodeUserFeed.h"
#include "net/SimpleID.h"
#include "ServerChannel.h"
#include "sglobal.h"
#include "tools/Ver.h"

NodeUserFeed::NodeUserFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  m_header.acl().setMask(0444);
}


NodeUserFeed::NodeUserFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  m_header.acl().setMask(0444);
}


QVariantMap NodeUserFeed::feed(Channel *channel) const
{
  if (head().channel()->type() != SimpleID::UserId || !can(channel, Acl::Read))
    return QVariantMap();

  QVariantMap out;
  QVariantMap connections;

  ServerChannel *ch = static_cast<ServerChannel *>(head().channel());
  const QMap<ChatId, HostInfo> &hosts = ch->hosts()->all();
  foreach (const HostInfo &info, hosts) {
    if (info->online)
      connections[info->hostId.toString()] = toMap(info);
  }

  out[LS("provider")]    = "simpleid";
  out[LS("connections")] = connections;
  out[LS("groups")]      = LS("registered");

  if (connections.isEmpty() && m_data.contains(LS("last")))
    merge(LS("last"), out, toMap(hosts.value(SimpleID::decode(m_data.value(LS("last")).toString()))));

  return out;
}


QVariantMap NodeUserFeed::toMap(HostInfo host) const
{
  if (!host)
    return QVariantMap();

  QVariantMap out;
  out[LS("host")]    = host->ip;
  out[LS("version")] = Ver(host->version).toString();
  out[LS("os")]      = host->os;
  out[LS("osName")]  = host->osName;
  out[LS("tz")]      = host->tz;

  merge(LS("geo"), out, host->geo);

  return out;
}


Feed* NodeUserFeedCreator::create(const QString &name) const
{
  return new NodeUserFeed(name, DateTime::utc());
}


Feed* NodeUserFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeUserFeed(name, data);
}
