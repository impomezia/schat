/* $Id: NodeAuthReply.cpp 3503 2013-02-15 11:44:58Z IMPOMEZIA $
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

#include <QUrl>

#include "Ch.h"
#include "cores/NodeAuth.h"
#include "feeds/ServerFeed.h"
#include "net/NodeAuthReply.h"
#include "sglobal.h"
#include "Storage.h"

NodeAuthReply::NodeAuthReply(const AuthRequest &request, const AuthResult &result, ChatChannel user)
  : AuthReply()
{
  flags      = 0;
  serverId   = Storage::serverId();
  status     = result.status;
  id         = result.authId;
  json       = result.json;

  ChatChannel server = Ch::server();
  serverName = server->name();

  FeedPtr feed              = server->feed(FEED_NAME_SERVER);
  const QStringList methods = feed->data().value(SERVER_FEED_AUTH_KEY).toStringList();

  if (methods.contains(AUTH_METHOD_ANONYMOUS))
    flags = 1;

  if (user) {
    userId   = user->id();
//    cookie   = user->account()->cookie; // FIXME: account()
    hostId   = user->hosts()->id();
    provider = LS("simpleid");

    const QString name = path(request.host);
    if (!name.isEmpty()) {
      ChatChannel c = Ch::channel(name, user);
      if (c) {
        channel  = c->id();
        policy   = ServerFeed::MainChannelPolicy | ServerFeed::ForcedJoinPolicy;
      }
    }

    if (channel.isEmpty()) {
      channel  = SimpleID::decode(feed->data().value(SERVER_FEED_CHANNEL_KEY).toString());
      policy   = feed->data().value(SERVER_FEED_POLICY_KEY).toInt();
    }
  }
  else if (methods.contains(AUTH_METHOD_OAUTH))
    provider = feed->data().value(SERVER_FEED_OAUTH_KEY).toString();
}


QString NodeAuthReply::path(const QUrl &url) const
{
  QString path = url.path();
  if (path.startsWith(LC('/')))
    path.remove(0, 1);

  return path;
}
