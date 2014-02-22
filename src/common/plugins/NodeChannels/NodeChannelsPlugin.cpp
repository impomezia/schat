/* $Id: NodeChannelsPlugin.cpp 3595 2013-03-24 22:05:32Z IMPOMEZIA $
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

#include "feeds/AutoKick.h"
#include "feeds/FeedStorage.h"
#include "feeds/NodeInfoFeed.h"
#include "feeds/NodeListFeed.h"
#include "feeds/NodeUsersFeed.h"
#include "NodeChannelIndex.h"
#include "NodeChannelsCh.h"
#include "NodeChannelsPlugin.h"
#include "NodeChannelsPlugin_p.h"

NodeChannelsImpl *NodeChannelsImpl::m_self = 0;

NodeChannelsImpl::NodeChannelsImpl(QObject *parent)
  : NodePlugin(parent)
{
  m_self = this;

  m_index = new NodeChannelIndex(this);

  new NodeChannelsCh(this);
  new AutoKick(this);

  FeedStorage::add(new NodeInfoFeedCreator());
  FeedStorage::add(new NodeListFeedCreator());
  FeedStorage::add(new NodeUsersFeedCreator());
}


NodePlugin *NodeChannelsPlugin::create()
{
  m_plugin = new NodeChannelsImpl(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(NodeChannels, NodeChannelsPlugin);
#endif
