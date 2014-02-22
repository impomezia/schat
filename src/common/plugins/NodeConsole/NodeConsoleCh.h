/* $Id: NodeConsoleCh.h 3190 2012-10-17 21:27:42Z IMPOMEZIA $
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

#ifndef NODECONSOLECH_H_
#define NODECONSOLECH_H_

#include "ChHook.h"

class NodeConsoleCh : public ChHook
{
  Q_OBJECT

public:
  NodeConsoleCh(QObject *parent = 0);
  void server(ChatChannel channel, bool created);
};

#endif /* NODECONSOLECH_H_ */
