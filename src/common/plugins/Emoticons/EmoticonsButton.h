/* $Id: EmoticonsButton.h 2716 2012-05-28 18:50:44Z IMPOMEZIA $
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

#ifndef EMOTICONSBUTTON_H_
#define EMOTICONSBUTTON_H_

#include <QApplication>
#include <QToolButton>

#include "ui/ToolBarAction.h"

class Emoticons;
class EmoticonsTabs;

class EmoticonsButton : public QToolButton
{
  Q_OBJECT

public:
  EmoticonsButton(Emoticons *emoticons, QWidget *parent = 0);

protected:
  void changeEvent(QEvent *event);

private slots:
  void menuAboutToHide();
  void menuAboutToShow();

private:
  void retranslateUi();

  Emoticons *m_emoticons; ///< Указатель на менеджер смайлов.
  QMenu *m_menu;          ///< Меню для показа виджета выбора смайлов.
};


class EmoticonsAction : public ToolBarActionCreator
{
  Q_DECLARE_TR_FUNCTIONS(EmoticonsAction)

public:
  EmoticonsAction(Emoticons *emoticons);
  QWidget* createWidget(QWidget *parent) const;
  QIcon icon() const;
  QString title() const;

private:
  Emoticons *m_emoticons;
};

#endif /* EMOTICONSBUTTON_H_ */
