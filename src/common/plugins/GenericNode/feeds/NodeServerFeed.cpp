/* $Id: NodeServerFeed.cpp 3735 2013-07-07 23:38:33Z IMPOMEZIA $
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

#include <QCoreApplication>

#include "Ch.h"
#include "Channel.h"
#include "DateTime.h"
#include "feeds/NodeServerFeed.h"
#include "feeds/ServerFeed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "Storage.h"
#include "tools/OsInfo.h"

NodeServerFeed::NodeServerFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
  , m_startupTime(0)
{
  init();
}


NodeServerFeed::NodeServerFeed(const QString &name, qint64 date)
  : Feed(name, date)
  , m_startupTime(0)
{
  init();
}


/*!
 * Обработка \b get запросов.
 *
 * Поддерживаемые запросы:
 * - \b uptime информация о дате запуска сервера и количестве секунд с момента запуска.
 */
FeedReply NodeServerFeed::get(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob) const
{
  Q_UNUSED(json)
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  if (path == SERVER_FEED_UPTIME_KEY) {
    FeedReply reply(Notice::OK);
    reply.json[SERVER_FEED_DATE_KEY]    = m_startupTime;
    reply.json[SERVER_FEED_SECONDS_KEY] = qAbs((DateTime::utc() - m_startupTime) / 1000);
    return reply;
  }

  return FeedReply(Notice::NotFound);
}


void NodeServerFeed::setChannel(Channel *channel)
{
  Feed::setChannel(channel);

  m_data[SERVER_FEED_NAME_KEY] = channel->name();
  m_data[SERVER_FEED_ID_KEY]   = SimpleID::encode(channel->id());
}


void NodeServerFeed::init()
{
  m_startupTime = DateTime::utc();
  m_header.acl().setMask(0444);
  m_header.setDate(m_startupTime);

  if (m_data.value(SERVER_FEED_VERSION_KEY) == LS("2.0.6"))
    m_data.remove(SERVER_FEED_OFFLINE_KEY);

  m_data[SERVER_FEED_VERSION_KEY] = QCoreApplication::applicationVersion();
  m_data[SERVER_FEED_OS_KEY]      = OsInfo::type();

  if (!m_data.contains(SERVER_FEED_OAUTH_KEY)) {
    m_data[SERVER_FEED_AUTH_KEY]  = QStringList() << AUTH_METHOD_ANONYMOUS << AUTH_METHOD_OAUTH;
    m_data[SERVER_FEED_OAUTH_KEY] = LS("https://auth.schat.me");
  }

  setDefault(SERVER_FEED_POLICY_KEY, ServerFeed::MainChannelPolicy | ServerFeed::ForcedJoinPolicy);
}


void NodeServerFeed::setDefault(const QString &key, const QVariant &value)
{
  if (!m_data.contains(key))
    m_data[key] = value;
}


Feed* NodeServerFeedCreator::create(const QString &name) const
{
  return new NodeServerFeed(name, DateTime::utc());
}


Feed* NodeServerFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeServerFeed(name, data);
}
