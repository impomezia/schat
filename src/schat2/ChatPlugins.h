/* $Id: ChatPlugins.h 2034 2011-12-24 12:15:57Z IMPOMEZIA $
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

#ifndef CHATPLUGINS_H_
#define CHATPLUGINS_H_

#include "Plugins.h"

class ChatPlugin;

class SCHAT_CORE_EXPORT ChatPlugins : public Plugins
{
  Q_OBJECT

public:
  ChatPlugins(QObject *parent = 0);

protected:
  void init();

private:
  QList<ChatPlugin *> m_chatPlugins;
};

#endif /* CHATPLUGINS_H_ */
