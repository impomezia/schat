/* $Id: TabBar.h 3699 2013-06-18 23:29:34Z IMPOMEZIA $
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

#ifndef TABBAR_H_
#define TABBAR_H_

#include <QObject>
#include <QTabBar>

class TabBar : public QTabBar
{
  Q_OBJECT

public:
  TabBar(QWidget *parent = 0);
  ButtonPosition closeButtonPosition() const;

protected:
  void mousePressEvent(QMouseEvent *event);
};

#endif /* TABBAR_H_ */
