/* $Id: ChannelsFeedListener.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include "ChannelsFeedListener.h"
#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientFeeds.h"
#include "feeds/FeedStrings.h"
#include "messages/AlertMessage.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"

ChannelsFeedListener::ChannelsFeedListener(QObject *parent)
  : QObject(parent)
{
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
}


void ChannelsFeedListener::notify(const Notify &notify)
{
  const int type = notify.type();

  if (type == Notify::FeedData || type == Notify::FeedReply) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if (SimpleID::typeOf(n.channel()) != SimpleID::ChannelId || n.command() != FEED_METHOD_GET)
      return;

    if (n.feed() == FEED_NAME_ACL && (type == Notify::FeedData || (type == Notify::FeedReply && n.status() == Notice::Forbidden))) {
      reload(n.channel(), n.status());
    }
  }
}


void ChannelsFeedListener::reload(const QByteArray &channelId, int status)
{
  ClientChannel channel = ChatClient::channels()->get(channelId);
  if (!channel)
    return;

  int color = Gender::Default;
  if (!ClientFeeds::match(channel) || status == Notice::Forbidden)
    color = Gender::Green;

  if (channel->gender().color() != color) {
    channel->gender().setColor(color);
    ChatNotify::start(Notify::UpdateChannelIcon, channelId, true);
  }

  if (status == Notice::Forbidden)
    AlertMessage::show(tr("<b>Access denied!</b> You cannot be in this channel."), ALERT_MESSAGE_ERROR, channelId);
}
