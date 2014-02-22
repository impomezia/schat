/* $Id: DiscoveryAuth.cpp 3467 2013-02-05 03:17:57Z IMPOMEZIA $
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
#include "cores/DiscoveryAuth.h"
#include "feeds/ServerFeed.h"
#include "net/packets/auth.h"
#include "sglobal.h"
#include "Storage.h"

DiscoveryAuth::DiscoveryAuth(Core *core)
  : AnonymousAuth(core)
{
}


AuthResult DiscoveryAuth::auth(const AuthRequest &data)
{
  const QStringList methods = Ch::server()->feed(FEED_NAME_SERVER)->data().value(SERVER_FEED_AUTH_KEY).toStringList();

  if (methods.contains(AUTH_METHOD_ANONYMOUS) && !methods.contains(AUTH_METHOD_OAUTH))
    return AnonymousAuth::auth(data);

  return AuthResult(Notice::Found, data.id);
}


int DiscoveryAuth::type() const
{
  return AuthRequest::Discovery;
}
