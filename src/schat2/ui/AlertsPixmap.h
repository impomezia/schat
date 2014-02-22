/* $Id: AlertsPixmap.h 2803 2012-06-27 19:41:01Z IMPOMEZIA $
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

#ifndef ALERTSPIXMAP_H_
#define ALERTSPIXMAP_H_

#include <QPixmap>
#include <QIcon>

#include "schat.h"

class SCHAT_CORE_EXPORT AlertsPixmap
{
  AlertsPixmap() {}

public:
  static int widthHint(int count);
  static QIcon icon(const QIcon &icon, int count);
  static QPixmap draw(int count);

private:
  static int pos(int number);
  static int width(int number);
  static void draw(int &offset, QPainter *painter, int number);
  static void draw(int x, int y, QPainter *painter, int number);
};

#endif /* ALERTSPIXMAP_H_ */
