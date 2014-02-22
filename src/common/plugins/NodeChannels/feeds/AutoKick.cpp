/* $Id: AutoKick.cpp 3643 2013-04-17 10:50:31Z IMPOMEZIA $
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

#include "acl/AclValue.h"
#include "Ch.h"
#include "cores/Core.h"
#include "feeds/AclFeed.h"
#include "feeds/AutoKick.h"
#include "feeds/FeedEvents.h"
#include "feeds/FeedStrings.h"
#include "net/Channels.h"
#include "net/packets/ChannelNotice.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "Sockets.h"
#include "feeds/ServerFeed.h"
#include "DataBase.h"

AutoKick::AutoKick(QObject *parent)
  : QObject(parent)
{
  connect(FeedEvents::i(), SIGNAL(notify(FeedEvent)), SLOT(notify(FeedEvent)));
}


void AutoKick::notify(const FeedEvent &event)
{
  if (event.status != Notice::OK || event.name != FEED_NAME_ACL || event.method == FEED_METHOD_GET || SimpleID::typeOf(event.channel) != SimpleID::ChannelId)
    return;

  const QVariant value = event.request.value(FEED_KEY_VALUE);
  ChatChannel channel  = Ch::channel(event.channel, SimpleID::ChannelId, false);
  if (!channel)
    return;

  if (event.method == FEED_METHOD_POST) {
    if (event.path.startsWith(LS("head/other/")) && value == 0)
      kick(channel, SimpleID::decode(event.path.mid(11)));
    else if (event.path == ACL_FEED_KICK_KEY) {
      kick(channel, SimpleID::decode(event.request.value(FEED_KEY_VALUE).toString()));
    }
  }
  else if (event.method == FEED_METHOD_DELETE && event.path.startsWith(LS("head/other/"))) {
    FeedPtr feed = channel->feed(FEED_NAME_ACL, false);
    if (feed && !AclValue::match(feed.data(), 0))
      kick(channel, SimpleID::decode(event.path.mid(11)));
  }
  else if (event.method == FEED_METHOD_PUT && event.path == LS("head/mask")) {
    kickAll(channel);
  }
}


void AutoKick::dump(ChatChannel channel) const
{
  if (Ch::server()->feed(FEED_NAME_SERVER)->data().value(SERVER_FEED_OFFLINE_KEY, true).toBool())
    DataBase::setValue(SimpleID::encode(channel->id()) + LS("/users"), channel->channels().join() + channel->offline().join());
}


/*!
 * Отключение одиночного пользователя.
 */
void AutoKick::kick(ChatChannel channel, const QByteArray &userId)
{
  ChatChannel user = Ch::channel(userId, SimpleID::UserId, false);
  if (!user)
    return;

  user->removeChannel(channel->id());
  if (channel->channels().contains(userId) || channel->offline().contains(userId)) {
    Core::i()->send(Sockets::channel(channel), ChannelNotice::request(userId, channel->id(), CHANNELS_PART_CMD));
    channel->removeChannel(userId);

    dump(channel);
    Ch::gc(channel);
  }
}


/*!
 * Отключение всех пользователей, которые не могут находиться в этом канале.
 */
void AutoKick::kickAll(ChatChannel channel)
{
  FeedPtr feed = channel->feed(FEED_NAME_ACL, false);
  if (!feed || AclValue::match(feed.data(), 0))
    return;

  const QList<QByteArray>& channels = channel->channels().all();
  const QList<quint64> sockets      = Sockets::channel(channel);
  QList<QByteArray> packets;

  foreach (const QByteArray &id, channels) {
    ChatChannel user = Ch::channel(id, SimpleID::UserId, false);
    if (user && !AclValue::match(feed.data(), user.data())) {
      user->removeChannel(channel->id());
      packets.append(ChannelNotice::request(user->id(), channel->id(), CHANNELS_PART_CMD)->data(Core::stream()));
      channel->removeChannel(user->id());
    }
  }

  dump(channel);
  Core::i()->send(sockets, packets);
  Ch::gc(channel);
}
