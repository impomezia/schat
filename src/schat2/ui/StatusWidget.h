/* $Id: StatusWidget.h 1976 2011-12-12 18:45:29Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2011 IMPOMEZIA <schat@impomezia.com>
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

#ifndef STATUSWIDGET_H_
#define STATUSWIDGET_H_

#include <QLabel>

class StatusWidget: public QLabel
{
  Q_OBJECT

public:
  StatusWidget(QWidget *parent = 0);

protected:
  void mouseReleaseEvent(QMouseEvent *event);

private slots:
  void reload();
};

#endif /* STATUSWIDGET_H_ */
