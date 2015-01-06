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
#include "cores/Core.h"
#include "feeds/ChannelFeed.h"
#include "feeds/FeedsCore.h"
#include "feeds/FeedStorage.h"
#include "feeds/FeedStrings.h"
#include "feeds/ServerFeed.h"
#include "GenericCh.h"
#include "sglobal.h"
#include "SJMPPacket.h"

GenericCh::GenericCh(QObject *parent)
  : ChHook(parent)
{
  connect(Core::i(), SIGNAL(packet(SJMPPacket)), SLOT(onPacket(SJMPPacket)));
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


void GenericCh::onPacket(const SJMPPacket &packet)
{
  if (packet.type() != SJMPPacket::Modified && !packet.resource().startsWith(LS("user/")))
    return;

  ChatChannel user = Ch::user(packet.resource().mid(5));
  if (!user)
    return;

  const QVariantMap body = packet.body().toMap();

  if (user->name() != body.value(LS("nick"))) {
    user->setName(body.value(LS("nick")).toString());
    Ch::rename(user, user->name());
    user->broadcast(FEED_NAME_CHANNEL, CHANNEL_FEED_NAME_KEY, packet.date());
  }

  const int gender = user->gender().raw();
  user->gender().set(body);
  if (gender != user->gender().raw()) {
    user->broadcast(FEED_NAME_CHANNEL, CHANNEL_FEED_GENDER_KEY, packet.date());
  }


  if (user->feed(FEED_NAME_PROFILE)->head().date() != packet.date()) {
    user->setData(LS("profile"), body);
    user->broadcast(FEED_NAME_PROFILE, QString(), packet.date());
  }
}
