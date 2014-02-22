/* $Id: AclValue.cpp 3590 2013-03-23 21:46:19Z IMPOMEZIA $
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

#include "Account.h"
#include "acl/Acl.h"
#include "acl/AclValue.h"
#include "Channel.h"
#include "feeds/Feed.h"
#include "net/SimpleID.h"
#include "sglobal.h"

AclValue::AclValue(const QByteArray &mask)
  : m_mask(toInt(mask))
{
}


AclValue::AclValue(const QVariant &mask)
{
  if (mask.type() == QVariant::String)
    m_mask = toInt(mask.toString().toLatin1());
  else
    m_mask = mask.toInt();
}


AclValue::AclValue(int mask)
  : m_mask(mask)
{
}


QByteArray AclValue::toByteArray() const
{
  return toByteArray(m_mask);
}


/*!
 * Преобразование строкового представления прав доступа в число.
 */
int AclValue::toInt(const QByteArray &mask)
{
  if (mask.size() < 3)
    return 0;

  int out = 0;

  if (mask.at(0) == 'r')
    out |= Acl::Read;
  else if (mask.at(0) == 'R')
    out |= Acl::Read | Acl::SpecialRead;

  if (mask.at(1) == 'w')
    out |= Acl::Write;
  else if (mask.at(1) == 'W')
    out |= Acl::Write | Acl::SpecialWrite;

  if (mask.at(2) == 'x')
    out |= Acl::Edit;
  else if (mask.at(2) == 'X')
    out |= Acl::Edit | Acl::SpecialEdit;

  return out;
}


/*!
 * Возвращает права доступа для конкретного пользователя.
 *
 * В отличии от Acl::match() поддерживается специальная обработка фида \b acl.
 */
int AclValue::match(const Feed *feed, Channel *channel)
{
  if (!feed)
    return 0;

  if (feed->head().name() == FEED_NAME_ACL) {
    if (channel && channel->type() == SimpleID::ServerId)
      return 077;

    if (channel && channel->account() && channel->account()->groups.contains(LS("master")))
      return 077;

    if (!feed->data().contains(FEED_WILDCARD_ASTERISK))
      return 6; // Необходимо для обеспечения обратной совместимости новых клиентов с сервером старее 1.99.53.

    const QByteArray mask = feed->data().value(FEED_WILDCARD_ASTERISK).toByteArray();
    if (!channel)
      return toInt(mask);

    return toInt(feed->data().value(SimpleID::encode(channel->id()), mask).toByteArray());
  }

  return feed->head().acl().match(channel);
}


/*!
 * Преобразование числового представления правд доступа в строку.
 */
QByteArray AclValue::toByteArray(int mask)
{
  QByteArray out("---");

  if (mask & Acl::Read)
    out[0] = mask & Acl::SpecialRead  ? 'R' : 'r';

  if (mask & Acl::Write)
    out[1] = mask & Acl::SpecialWrite ? 'W' : 'w';

  if (mask & Acl::Edit)
    out[2] = mask & Acl::SpecialEdit  ? 'X' : 'x';

  return out;
}
