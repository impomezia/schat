/* $Id: HandlerRoute.h 2837 2012-07-07 21:46:56Z IMPOMEZIA $
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

#ifndef HANDLERROUTE_H_
#define HANDLERROUTE_H_

#include <QList>

class HandlerCreator;

class HandlerRoute
{
public:
  HandlerRoute() {}
  ~HandlerRoute();

  static QList<HandlerCreator *> routes;
};

#endif /* HANDLERROUTE_H_ */
