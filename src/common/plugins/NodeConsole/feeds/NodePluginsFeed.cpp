/* $Id: NodePluginsFeed.cpp 3420 2013-01-19 05:23:36Z IMPOMEZIA $
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
#include "feeds/NodePluginsFeed.h"
#include "NodePlugins.h"
#include "sglobal.h"

NodePluginsFeed::NodePluginsFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  init();
  m_header.setDate();
}


NodePluginsFeed::NodePluginsFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  init();
}


void NodePluginsFeed::init()
{
  m_header.acl().setMask(0);

  foreach (PluginItem *item, NodePlugins::i()->list()) {
    if (!item->isLoaded())
      continue;

    QVariantMap data;
    const QVariantMap &header = item->header();

    data[CORE_API_NAME]    = header.value(CORE_API_NAME);
    data[CORE_API_VERSION] = header.value(CORE_API_VERSION);
    m_data[item->id()] = data;
  }
}


Feed* NodePluginsFeedCreator::create(const QString &name) const
{
  return new NodePluginsFeed(name, DateTime::utc());
}


Feed* NodePluginsFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodePluginsFeed(name, data);
}
