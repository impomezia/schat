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

#ifndef USERITEM_H_
#define USERITEM_H_

#include <QStandardItem>

#include "Channel.h"

/*!
 * Итем в списке пользователей.
 */
class UserItem : public QStandardItem
{
public:
  UserItem(ClientChannel user, ClientChannel channel);
  inline ClientChannel user() { return m_user; }
  bool reload();

private:
  int weight() const;
  QBrush color() const;

  bool m_bold;             ///< Жирный шрифт.
  bool m_italic;           ///< Курсивный шрифт.
  bool m_self;             ///< \b true если это собственный итем пользователя.
  bool m_underline;        ///< Подчёркнутый шрифт.
  ClientChannel m_channel; ///< Канал.
  ClientChannel m_user;    ///< Пользователь.
};

#endif // USERITEM_H
