/* $Id: NodeConsolePlugin.cpp 3420 2013-01-19 05:23:36Z IMPOMEZIA $
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

#include <QtPlugin>

#include "feeds/FeedStorage.h"
#include "feeds/NodeConsoleFeed.h"
#include "feeds/NodePluginsFeed.h"
#include "feeds/NodeStorageFeed.h"
#include "NodeConsoleCh.h"
#include "NodeConsolePlugin.h"
#include "NodeConsolePlugin_p.h"
#include "PasswordHook.h"
#include "Storage.h"

NodeConsoleImpl::NodeConsoleImpl(QObject *parent)
  : NodePlugin(parent)
{
  new NodeConsoleCh(this);

  FeedStorage::add(new NodeConsoleFeedCreator());
  FeedStorage::add(new NodeStorageFeedCreator());
  FeedStorage::add(new NodePluginsFeedCreator());

  Storage::add(new PasswordHook());
}


NodePlugin *NodeConsolePlugin::create()
{
  m_plugin = new NodeConsoleImpl(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(NodeConsole, NodeConsolePlugin);
#endif
