/* $Id: SimpleID.cpp 3708 2013-06-23 23:40:16Z IMPOMEZIA $
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

#include "id/ChatId.h"
#include "net/SimpleID.h"
#include "sglobal.h"

/*!
 * Возвращает тип идентификатора.
 */
int SimpleID::typeOf(const QByteArray &id)
{
  return ChatId(id).type();
}


/*!
 * Декодирование идентификатора из Base32.
 *
 * \param id Идентификатор, размер должен быть равен EncodedSize.
 * \return Кодированный идентификатор или пустой массив в случае ошибки.
 */
QByteArray SimpleID::decode(const QByteArray &id)
{
  return ChatId(id).toByteArray();
}


QByteArray SimpleID::decode(const QString &id)
{
  return decode(id.toLatin1());
}


/*!
 * Кодирование идентификатора в Base32.
 *
 * Если размер входного идентификатора равен 34, делается предположение что идентификатор уже кодирован и он возвращается без изменения.
 *
 * \param id Идентификатор, размер должен быть равен DefaultSize.
 * \return Кодированный идентификатор или пустой массив в случае ошибки.
 */
QByteArray SimpleID::encode(const QByteArray &id)
{
  return ChatId(id).toBase32();
}


/*!
 * Создание идентификатора.
 *
 * \param data Данные на основе которых будет создан идентификатор.
 * \param type Тип идентификатора.
 */
QByteArray SimpleID::make(const QByteArray &data, Types type)
{
  return ChatId(data, static_cast<ChatId::Type>(type)).toByteArray();
}


QString SimpleID::typeName(int type)
{
  switch (type) {
    case UniqueUserId:
      return LS("unique");

    case UserId:
      return LS("user");

    case ServerId:
      return LS("server");

    case CookieId:
      return LS("cookie");

    case ChannelId:
      return LS("channel");

    case MessageId:
      return LS("message");

    case PasswordId:
      return LS("password");

    case NormalizedId:
      return LS("normalized");

    case HostId:
      return LS("host");

    default:
      return LS("invalid");
  }
}
