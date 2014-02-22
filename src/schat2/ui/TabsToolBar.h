/* $Id: TabsToolBar.h 3225 2012-10-27 08:36:51Z IMPOMEZIA $
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

#ifndef TABSTOOLBAR_H_
#define TABSTOOLBAR_H_

#include <QToolBar>

#include "schat.h"

class QMenu;
class QToolButton;

class SCHAT_CORE_EXPORT TabsToolBar : public QToolBar
{
  Q_OBJECT

public:
  TabsToolBar(QWidget *parent = 0);
  inline QMenu *menu() { return m_menu; }

signals:
  void showMenu(QMenu *menu, QAction *separator);

protected:
  void changeEvent(QEvent *event);

private slots:
  void hideMenu();
  void showMenu();

private:
  void retranslateUi();

  QMenu *m_menu;         ///< Главное меню.
  QMenu *m_talks;        ///< Меню разговоров.
  QToolButton *m_button; ///< Кнопка с меню пользователей и каналов.
};

#endif /* TABSTOOLBAR_H_ */
