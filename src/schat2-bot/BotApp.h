/* $Id: BotApp.h 1545 2011-07-18 18:00:59Z IMPOMEZIA $
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

#ifndef BOTAPP_H_
#define BOTAPP_H_

#include <QCoreApplication>

class BotPlugins;

class BotApp : public QCoreApplication
{
  Q_OBJECT

public:
  BotApp(int &argc, char **argv);

private slots:
  void start();

private:
  BotPlugins *m_plugins;
};

#endif /* BOTAPP_H_ */
