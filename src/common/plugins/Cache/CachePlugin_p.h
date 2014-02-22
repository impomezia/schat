/* $Id: CachePlugin_p.h 2610 2012-05-06 00:04:20Z IMPOMEZIA $
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

#ifndef CACHEPLUGIN_P_H_
#define CACHEPLUGIN_P_H_

#include "Channel.h"
#include "plugins/ChatPlugin.h"

class Notify;

class Cache : public ChatPlugin
{
  Q_OBJECT

public:
  Cache(QObject *parent);

private slots:
  void notify(const Notify &notify);
  void open();
  void ready();

private:
  void load(ClientChannel channel);
  void loadCache();
};

#endif /* CACHEPLUGIN_P_H_ */
