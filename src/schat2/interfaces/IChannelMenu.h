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

#ifndef ICHANNELMENU_H_
#define ICHANNELMENU_H_

#include "Channel.h"

class QMenu;
class QAction;

class IChannelMenu
{
public:
  enum Scope {
    UnknownScope,   ///< Неизвестное место.
    StatusBarScope, ///< Статус бар.
    TabScope,       ///< Заголовок вкладки.
    UserViewScope,  ///< Список пользователей.
    ChatViewScope   ///< Текст чата.
  };

  virtual ~IChannelMenu() {}
  virtual bool trigger(QAction *action) = 0;
  virtual void bind(QMenu *menu, ClientChannel channel, Scope scope) = 0;
  virtual void cleanup() = 0;
};

#endif // ICHANNELMENU_H_
