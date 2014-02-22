/* $Id: HistoryMessages.h 3095 2012-09-17 02:11:22Z IMPOMEZIA $
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

#ifndef HISTORYMESSAGES_H_
#define HISTORYMESSAGES_H_

#include "client/ClientHooks.h"

class HistoryMessages : public Hooks::Messages
{
  Q_OBJECT

public:
  HistoryMessages(QObject *parent = 0);

  int read(MessagePacket packet);
};

#endif /* HISTORYMESSAGES_H_ */
