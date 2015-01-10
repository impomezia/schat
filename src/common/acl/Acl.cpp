/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#include "acl/Acl.h"
#include "Channel.h"
#include "net/SimpleID.h"
#include "sglobal.h"

Acl::Acl(int mask)
  : m_mask(mask)
{
}


bool Acl::add(const QByteArray &other, int acl)
{
  if (SimpleID::typeOf(other) != SimpleID::UserId || acl < 0 || acl > 077)
    return false;

  m_others[other] = acl;
  return true;
}


/*!
 * Получение данных о правах доступа.
 */
bool Acl::get(QVariantMap &data, Channel *channel) const
{
  const int acl = match(channel);

  if (!(acl & Read))
    return false;

  if (acl & Edit)
    save(data);

  data[LS("match")] = acl;
  data[ACL_MASK]  = m_mask;
  return true;
}


/*!
 * Возвращает права доступа для конкретного пользователя.
 *
 * \param channel Канал-пользователь.
 * \return Acl::Mask.
 */
int Acl::match(Channel *channel) const
{
  if (!channel)
    return (m_mask & ~0770);

  if (channel->type() == SimpleID::ServerId)
    return 077;

  // FIXME: L4SGc3F
  if (channel->nativeId() == LS("L4SGc3F"))
    return 077;

  const QByteArray &id = channel->id();

  if (m_owners.contains(id))
    return m_mask >> 6 | SpecialEdit;

  if (m_others.contains(id))
    return m_others.value(id);

  return (m_mask & ~0770);
}


void Acl::add(const QByteArray &owner)
{
  if (SimpleID::typeOf(owner) != SimpleID::UserId || m_owners.contains(owner))
    return;

  m_owners.append(owner);
}


void Acl::load(const QVariantMap &json)
{
  m_mask = json.value(ACL_MASK).toInt();
  m_others.clear();
  m_owners.clear();

  const QVariantList owners = json.value(ACL_OWNERS).toList();
  foreach (const QVariant &owner, owners)
    add(SimpleID::decode(owner.toByteArray()));

  const QVariantMap others = json.value(ACL_OTHERS).toMap();
  QMapIterator<QString, QVariant> i(others);
  while (i.hasNext()) {
    i.next();
    add(SimpleID::decode(i.key()), i.value().toInt());
  }
}


/*!
 * Сохранение информации о правах доступа.
 */
void Acl::save(QVariantMap &data) const
{
  QVariantList owners;
  foreach (const QByteArray &owner, m_owners)
    owners.append(SimpleID::encode(owner));

  QVariantMap others;
  QMapIterator<QByteArray, int> i(m_others);
  while (i.hasNext()) {
    i.next();
    others[SimpleID::encode(i.key())] = i.value();
  }

  data[ACL_MASK]   = m_mask;
  data[ACL_OWNERS] = owners;
  data[ACL_OTHERS] = others;
}
