/* $Id: ConsolePlugin_p.h 3459 2013-02-02 06:07:07Z IMPOMEZIA $
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

#ifndef CONSOLEPLUGIN_P_H_
#define CONSOLEPLUGIN_P_H_

#include "plugins/ChatPlugin.h"

class ConsoleTr;
class QAction;
class QMenu;

class ConsolePluginImpl : public ChatPlugin
{
  Q_OBJECT

public:
  ConsolePluginImpl(QObject *parent);
  ~ConsolePluginImpl();

public slots:
  void show();

private slots:
  void showMenu(QMenu *menu, QAction *separator);
  void start();

private:
  ConsoleTr *m_tr;     ///< Перевод.
  QAction *m_console;  ///< Пункт меню для показа вкладки консоли.
};

#endif /* CONSOLEPLUGIN_P_H_ */
