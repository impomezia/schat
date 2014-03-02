/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

#ifndef MAINTOOLBAR_H_
#define MAINTOOLBAR_H_

#include <QToolBar>

class QMenu;
class QToolButton;
class SoundButton;

class MainToolBar : public QToolBar
{
  Q_OBJECT

public:
  MainToolBar(QWidget *parent = 0);
  void setReady(bool ready);

protected:
  void changeEvent(QEvent *event);

private slots:
  void triggered(QAction *action);

private:
  void retranslateUi();

  QAction *m_about;      ///< О Simple Chat.
  QAction *m_quit;       ///< Выход из чата.
  QAction *m_settings;   ///< Настройки.
  QMenu *m_menu;         ///< Меню настройки.
  QToolButton *m_button; ///< Кнопка с меню настроек.
  SoundButton *m_sound;  ///< Кнопка включения/выключения звука.
};

#endif /* MAINTOOLBAR_H_ */
