/* $Id: NodeChannelsCh.cpp 3443 2013-01-26 00:27:36Z IMPOMEZIA $
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
#include "NodeChannelsCh.h"
#include "sglobal.h"
#include "Storage.h"

NodeChannelsCh::NodeChannelsCh(QObject *parent)
  : ChHook(parent)
{
}


void NodeChannelsCh::load()
{
  QStringList permanent = Storage::value(LS("PermanentChannels")).toStringList();

  foreach (const QString &id, permanent) {
    ChatChannel channel = Ch::channel(SimpleID::decode(id), SimpleID::ChannelId);
    if (channel)
      channel->setPermanent(true);
  }
}


void NodeChannelsCh::newChannel(ChatChannel channel, ChatChannel user)
{
  Q_UNUSED(user)
  Ch::addNewFeedIfNotExist(channel, FEED_NAME_INFO);
  channel->feed(FEED_NAME_USERS);
}


void NodeChannelsCh::server(ChatChannel channel, bool created)
{
  Q_UNUSED(created)
  channel->feed(FEED_NAME_LIST);
  channel->feed(FEED_NAME_USERS);
}


void NodeChannelsCh::sync(ChatChannel channel, ChatChannel user)
{
  Q_UNUSED(user)
  Ch::addNewFeedIfNotExist(channel, FEED_NAME_INFO);
  channel->feed(FEED_NAME_USERS);
}
