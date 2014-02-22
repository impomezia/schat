/* $Id: ServerFeed.h 3632 2013-04-12 04:44:28Z IMPOMEZIA $
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

#ifndef SERVERFEED_H_
#define SERVERFEED_H_

#define SERVER_FEED_AUTH_KEY    QLatin1String("auth")
#define SERVER_FEED_CHANNEL_KEY QLatin1String("channel")
#define SERVER_FEED_CHANNEL_REQ QLatin1String("server/channel")
#define SERVER_FEED_DATE_KEY    QLatin1String("date")
#define SERVER_FEED_ID_KEY      QLatin1String("id")
#define SERVER_FEED_NAME_KEY    QLatin1String("name")
#define SERVER_FEED_NAME_REQ    QLatin1String("server/name")
#define SERVER_FEED_OAUTH_KEY   QLatin1String("oauth")
#define SERVER_FEED_OFFLINE_KEY QLatin1String("offline")
#define SERVER_FEED_OS_KEY      QLatin1String("os")
#define SERVER_FEED_POLICY_KEY  QLatin1String("policy")
#define SERVER_FEED_SECONDS_KEY QLatin1String("seconds")
#define SERVER_FEED_UPTIME_KEY  QLatin1String("uptime")
#define SERVER_FEED_VERSION_KEY QLatin1String("version")

#define AUTH_METHOD_ANONYMOUS   QLatin1String("anonymous")
#define AUTH_METHOD_OAUTH       QLatin1String("oauth")

class ServerFeed
{
public:
  /// Политика основного канала.
  enum Policy {
    NoPolicy             = 0, ///< Нет специальных политик, клиент при подключении будет показывать список каналов.
    MainChannelPolicy    = 1, ///< Включает поддержку главного канала.
    ForcedJoinPolicy     = 2, ///< Клиент всегда будет входить в главный канал.
    DisabledPartPolicy   = 4, ///< Если включен автоматический вход, клиент не сможет выйти из главного канала.
  };
};

#endif /* SERVERFEED_H_ */
