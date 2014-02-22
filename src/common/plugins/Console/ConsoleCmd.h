/* $Id: ConsoleCmd.h 3485 2013-02-10 09:59:44Z IMPOMEZIA $
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

#ifndef CONSOLECMD_H_
#define CONSOLECMD_H_

#include "hooks/MessagesHook.h"

class ConsolePluginImpl;
class Notify;

class ConsoleCmd : public MessagesHook
{
  Q_OBJECT

public:
  ConsoleCmd(ConsolePluginImpl *plugin);
  bool command(const QByteArray &dest, const ClientCmd &cmd);

private slots:
  void notify(const Notify &notify);

private:
  void getCookie(const QByteArray &id);

  ConsolePluginImpl *m_plugin; ///< Указатель на плагин.
  QByteArray m_cookie;         ///< Индентификатор пользователя для которого ожидается получение cookie.
};

#endif /* CONSOLECMD_H_ */
