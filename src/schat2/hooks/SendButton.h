/* $Id: SendButton.h 3764 2013-08-12 23:01:53Z IMPOMEZIA $
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

#ifndef SENDBUTTON_H_
#define SENDBUTTON_H_

#include <QApplication>
#include <QToolButton>

#include "ui/ToolBarAction.h"

class SendButton : public QToolButton
{
  Q_OBJECT

public:
  SendButton(QWidget *parent = 0);

protected:
  void changeEvent(QEvent *event);

private slots:
  void clear();
  void showItem();
  void showMenu();

private:
  void retranslateUi();

  QMenu *m_history; ///< Меню отправленных сообщений.
};


class SendAction : public ToolBarActionCreator
{
  Q_DECLARE_TR_FUNCTIONS(SendAction)

public:
  SendAction();
  QWidget* createWidget(QWidget *parent) const;
  QIcon icon() const;
  QString title() const;
};

#endif /* SENDBUTTON_H_ */
