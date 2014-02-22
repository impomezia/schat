/* $Id: MessagesCh.h 3755 2013-07-14 23:11:47Z IMPOMEZIA $
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

#ifndef MESSAGESCH_H_
#define MESSAGESCH_H_

#include "ChHook.h"

class MessagesCh : public ChHook
{
  Q_OBJECT

public:
  MessagesCh(QObject *parent = 0);
  void newChannel(ChatChannel channel, ChatChannel user = ChatChannel());
  void server(ChatChannel channel, bool created);
  void sync(ChatChannel channel, ChatChannel user = ChatChannel());
  void userChannel(ChatChannel channel);
};

#endif /* MESSAGESCH_H_ */
