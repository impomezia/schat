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

#include "AuthProxy.h"
#include "Ch.h"
#include "cores/AnonymousAuth.h"
#include "cores/Core.h"
#include "events.h"
#include "feeds/ServerFeed.h"
#include "net/packets/auth.h"
#include "sglobal.h"

QMap<QByteArray, quint64> AnonymousAuth::m_collisions;

AnonymousAuth::AnonymousAuth(Core *core)
  : NodeAuth(core)
{
}


AuthResult AnonymousAuth::auth(const AuthRequest &data)
{
  if (!Ch::server()->feed(FEED_NAME_SERVER)->data().value(SERVER_FEED_AUTH_KEY).toStringList().contains(AUTH_METHOD_ANONYMOUS))
    return AuthResult(Notice::NotImplemented, data.id);

  new AuthProxy(data, m_core->packetsEvent()->address.toString(), m_core);
  return AuthResult::Pending;
}


int AnonymousAuth::type() const
{
  return AuthRequest::Anonymous;
}


/*!
 * Проверка на коллизию ника.
 * Допускается не более 20 попыток разрешить коллизию без разрыва соединения.
 *
 * FIXME: Remove isCollision
 *
 * \param id       Идентификатор канала.
 * \param name     Имя канала.
 * \param authId   Идентификатор авторизации.
 * \param override \b true если допустима попытка безопасной перезаписи ника.
 */
AuthResult AnonymousAuth::isCollision(const QByteArray &id, const QString &name, const QByteArray &authId, bool override)
{
  quint64 collisions = m_collisions.value(id);

  if (Ch::isCollision(id, name, override)) {
    ++collisions;
    m_collisions[id] = collisions;

    if (collisions == 10)
      return AuthResult(Notice::NickAlreadyUse, authId);

    return AuthResult(Notice::NickAlreadyUse, authId, collisions > 20 ? 2 : 0);
  }

  m_collisions.remove(id);
  return AuthResult();
}


/*!
 * FIXME: Remove update
 *
 * \param channel
 * \param data
 */
void AnonymousAuth::update(ServerChannel *channel, const AuthRequest &data)
{
  if (channel->status().value() == Status::Offline) {
    channel->setDate();
    channel->status().set(data.status);
  }
}
