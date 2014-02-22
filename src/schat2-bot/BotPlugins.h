/* $Id: BotPlugins.h 1545 2011-07-18 18:00:59Z IMPOMEZIA $
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

#ifndef BOTPLUGINS_H_
#define BOTPLUGINS_H_

#include "Plugins.h"

class ClientHelper;
class FileLocations;
class SimpleClient;

class BotPlugins : public Plugins
{
  Q_OBJECT

public:
  BotPlugins(QObject *parent = 0);

protected:
  void init();

private:
  ClientHelper *m_helper;     ///< Обвёртка над клиентом.
  FileLocations *m_locations; ///< Схема размещения файлов.
  SimpleClient *m_client;     ///< Клиент.
};

#endif /* BOTPLUGINS_H_ */
