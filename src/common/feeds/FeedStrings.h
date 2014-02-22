/* $Id: FeedStrings.h 3692 2013-06-14 18:14:42Z IMPOMEZIA $
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

#ifndef FEEDSTRINGS_H_
#define FEEDSTRINGS_H_

#define FEED_METHOD_GET    QLatin1String("get")
#define FEED_METHOD_PUT    QLatin1String("put")
#define FEED_METHOD_POST   QLatin1String("post")
#define FEED_METHOD_DELETE QLatin1String("delete")
#define FEED_METHOD_FEED   QLatin1String("feed")

#define FEED_NAME_ACL      QLatin1String("acl")
#define FEED_NAME_CHANNEL  QLatin1String("channel")
#define FEED_NAME_CONSOLE  QLatin1String("console")
#define FEED_NAME_HOSTS    QLatin1String("hosts")
#define FEED_NAME_INFO     QLatin1String("info")
#define FEED_NAME_LIST     QLatin1String("list")
#define FEED_NAME_MESSAGES QLatin1String("messages")
#define FEED_NAME_PLUGINS  QLatin1String("plugins")
#define FEED_NAME_PROFILE  QLatin1String("profile")
#define FEED_NAME_SERVER   QLatin1String("server")
#define FEED_NAME_STORAGE  QLatin1String("storage")
#define FEED_NAME_USER     QLatin1String("user")
#define FEED_NAME_SETTINGS QLatin1String("settings")
#define FEED_NAME_USERS    QLatin1String("users")

#define FEED_WILDCARD_ASTERISK QLatin1String("*")

#define FEED_KEY_COMPACT QLatin1String("compact")
#define FEED_KEY_F       QLatin1String("f")
#define FEED_KEY_OPTIONS QLatin1String("options")
#define FEED_KEY_VALUE   QLatin1String("value")

#endif /* FEEDSTRINGS_H_ */
