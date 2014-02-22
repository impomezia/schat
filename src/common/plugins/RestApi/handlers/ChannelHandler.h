/* $Id: ChannelHandler.h 3019 2012-08-20 00:05:43Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#ifndef CHANNELHANDLER_H_
#define CHANNELHANDLER_H_

#include <QMap>

#include "RestHandler.h"
#include "ServerChannel.h"

class ChannelHandler : public RestHandler
{
public:
  ChannelHandler();
  bool serve();

private:
  bool badRequest();
  bool channel(ChatChannel channel);

  QMap<QByteArray, RestReplyCache> m_cache; ///< Кешированные ответы.
};

#endif /* CHANNELHANDLER_H_ */
