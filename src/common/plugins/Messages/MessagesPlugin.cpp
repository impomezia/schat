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

#include "cores/Core.h"
#include "feeds/FeedStorage.h"
#include "feeds/NodeMessagesFeed.h"
#include "MessagesCh.h"
#include "MessagesPlugin.h"
#include "MessagesPlugin_p.h"
#include "NodeMessages.h"
#include "NodeMessagesDB.h"

NodeMessagesBase::NodeMessagesBase(QObject *parent)
  : NodePlugin(parent)
{
  new NodeMessagesDB(this);
  m_messages = new NodeMessages(Core::i());
  new MessagesCh(this);
  FeedStorage::add(new NodeMessagesFeedCreator());

  NodeMessagesDB::open();
}


NodeMessagesBase::~NodeMessagesBase()
{
  delete m_messages;
}


MessagesPlugin::MessagesPlugin(QObject *parent)
  : QObject(parent)
{
}


NodePlugin *MessagesPlugin::create()
{
  m_plugin = new NodeMessagesBase(this);
  return m_plugin;
}
