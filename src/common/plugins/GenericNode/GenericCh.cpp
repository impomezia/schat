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
#include "feeds/FeedsCore.h"
#include "feeds/FeedStorage.h"
#include "feeds/FeedStrings.h"
#include "feeds/ServerFeed.h"
#include "GenericCh.h"
#include "sglobal.h"

GenericCh::GenericCh(QObject *parent)
  : ChHook(parent)
{
}


void GenericCh::newChannel(ChatChannel channel, ChatChannel user)
{
  Ch::addNewFeedIfNotExist(channel, FEED_NAME_ACL, user);
  Ch::addNewFeedIfNotExist(channel, FEED_NAME_CHANNEL, user);
}


void GenericCh::rename(ChatChannel channel)
{
  if (channel->type() == SimpleID::ServerId)
    FeedsCore::put(SERVER_FEED_NAME_REQ, channel->name());
}


void GenericCh::server(ChatChannel channel, bool created)
{
  Q_UNUSED(created)

  channel->feed(FEED_NAME_ACL);
  channel->feed(FEED_NAME_SERVER);
  channel->feed(FEED_NAME_CHANNEL);
}


void GenericCh::sync(ChatChannel channel, ChatChannel user)
{
  Ch::addNewFeedIfNotExist(channel, FEED_NAME_CHANNEL, user);
}


void GenericCh::userChannel(ChatChannel channel)
{
  Ch::addNewUserFeedIfNotExist(channel, FEED_NAME_ACL);
  Ch::addNewUserFeedIfNotExist(channel, FEED_NAME_PROFILE);
  Ch::addNewUserFeedIfNotExist(channel, FEED_NAME_CHANNEL);

  channel->feed(FEED_NAME_PROFILE, false, false)->head().setDate(channel->data().value(LS("profile")).toMap().value(LS("datetime")).toLongLong());
}
