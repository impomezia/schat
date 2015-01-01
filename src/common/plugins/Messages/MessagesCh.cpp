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

#include "Ch.h"
#include "MessagesCh.h"
#include "sglobal.h"

MessagesCh::MessagesCh(QObject *parent)
  : ChHook(parent)
{
}


void MessagesCh::newChannel(ChatChannel channel, ChatChannel user)
{
  Ch::addNewFeedIfNotExist(channel, FEED_NAME_MESSAGES, user);
}


void MessagesCh::server(ChatChannel channel, bool created)
{
  Q_UNUSED(created)

  channel->feed(FEED_NAME_MESSAGES);
}


void MessagesCh::sync(ChatChannel channel, ChatChannel user)
{
  Q_UNUSED(user)
  Ch::addNewFeedIfNotExist(channel, FEED_NAME_MESSAGES);
}


void MessagesCh::userChannel(ChatChannel channel)
{
  Ch::addNewUserFeedIfNotExist(channel, FEED_NAME_MESSAGES);
}
