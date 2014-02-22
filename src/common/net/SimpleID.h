/* $Id: SimpleID.h 3721 2013-07-02 00:44:36Z IMPOMEZIA $
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

#ifndef SIMPLEID_H_
#define SIMPLEID_H_

#include <QByteArray>

#include "schat.h"

/*!
 * Работка с идентификаторами.
 *
 * \deprecated Этот класс должен быть заменён на ChatId.
 */
class SCHAT_EXPORT SimpleID
{
public:
  /// Значения по умолчанию.
  enum Defaults {
    DefaultSize = 21, ///< Размер идентификатора в байтах.
    EncodedSize = 34  ///< Размер идентификатора кодированного в Base32.
  };

  /// Типы идентификаторов.
  enum Types {
    InvalidId    = 0,      ///< 'I' Неверный идентификатор.
    UniqueUserId = 85,     ///< 'U' Уникальный идентификатор пользователя, использующийся при авторизации.
    UserId       = 117,    ///< 'u' Стандартный идентификатор пользователя.
    ServerId     = 83,     ///< 'S' Идентификатор сервера.
    CookieId     = 67,     ///< 'C' Идентификатор Cookie.
    ChannelId    = 99,     ///< 'c' Идентификатор канала.
    MessageId    = 109,    ///< 'm' Идентификатор сообщения.
    PasswordId   = 112,    ///< 'p' Идентификатор пароля.
    NormalizedId = 110,    ///< 'n' Идентификатор нормализированного имени канала.
    HostId       = 72      ///< 'H' Идентификатор хоста.
  };

  SimpleID() {}
  static int typeOf(const QByteArray &id);
  static QByteArray decode(const QByteArray &id);
  static QByteArray decode(const QString &id);
  static QByteArray encode(const QByteArray &id);
  static QByteArray make(const QByteArray &data, Types type);
  static QString typeName(int type);
};

#endif /* SIMPLEID_H_ */
