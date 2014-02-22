/* $Id: Account.h 2928 2012-07-27 18:17:44Z IMPOMEZIA $
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

#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include "acl/Groups.h"

#include <QString>

/*!
 * Информация об аккаунте пользователя.
 */
class Account
{
public:
  inline Account()
  : saved(true)
  , channel(0)
  , date(0)
  , flags(0)
  {}

  inline void setDate(qint64 date) { saved = false; this->date = date; }

  bool saved;        ///< \b true если данные сохранены.
  Groups groups;     ///< Группы пользователя.
  QByteArray cookie; ///< Сookie пользователя.
  qint64 channel;    ///< Идентификатор канала в таблице channels.
  qint64 date;       ///< Дата обновления.
  qint64 flags;      ///< Дополнительные флаги.
  QString provider;  ///< Авторизационной провайдер пользователя.
};

#endif /* ACCOUNT_H_ */
