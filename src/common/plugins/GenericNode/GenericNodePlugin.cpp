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

#include <QtPlugin>

#include "cores/Core.h"
#include "feeds/NodeFeedStorage.h"
#include "GenericCh.h"
#include "GenericNodePlugin.h"
#include "GenericNodePlugin_p.h"
#include "NodeChannels.h"
#include "NodeFeeds.h"
#include "sglobal.h"

GenericNode::GenericNode(QObject *parent)
  : NodePlugin(parent)
{
  new NodeChannels(Core::i());
  m_feeds = new NodeFeeds(Core::i());
  new GenericCh(this);
  new NodeFeedStorage(this);
}


GenericNode::~GenericNode()
{
  delete m_feeds;
}


NodePlugin *GenericNodePlugin::create()
{
  m_plugin = new GenericNode(this);
  return m_plugin;
}


Q_EXPORT_PLUGIN2(GenericNode, GenericNodePlugin);
