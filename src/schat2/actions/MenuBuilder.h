/* $Id: MenuBuilder.h 2004 2011-12-16 14:03:58Z IMPOMEZIA $
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

#ifndef MENUBUILDER_H_
#define MENUBUILDER_H_

#include <QObject>

#include "schat.h"

class QAction;
class QMenu;

class SCHAT_CORE_EXPORT MenuBuilder : public QObject
{
  Q_OBJECT

public:
  MenuBuilder(QObject *parent = 0);
  ~MenuBuilder();
  virtual void bind(QMenu *menu);
  virtual void retranslateUi() {}

protected slots:
  virtual void triggered(QAction *action);

protected:
  virtual void bindImpl();

  QMenu *m_menu; ///< Меню с которым происходит связывание.
};

#endif /* MENUBUILDER_H_ */
