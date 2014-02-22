/* $Id: Sockets.h 3443 2013-01-26 00:27:36Z IMPOMEZIA $
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

#ifndef SOCKETS_H_
#define SOCKETS_H_

#include "ServerChannel.h"

class SCHAT_EXPORT Sockets
{
  Sockets() {}

public:
  static QList<quint64> all(ChatChannel channel, ChatChannel user, bool echo = false);
  static QList<quint64> all(ChatChannel user, bool echo = false);
  static QList<quint64> channel(ChatChannel channel);
  static QList<quint64> channel(ServerChannel *channel);
  static void echoFilter(ChatChannel channel, QList<quint64> &sockets, bool echo);
  static void merge(QList<quint64> &out, const QList<quint64> &sockets);
};

#endif /* SOCKETS_H_ */
