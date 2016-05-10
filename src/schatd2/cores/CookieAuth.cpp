/* Simple Chat
 * Copyright (c) 2008-2016 Alexander Sedov <imp@schat.me>
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
#include "Ch.h"
#include "cores/CookieAuth.h"
#include "cores/Core.h"
#include "events.h"
#include "net/packets/auth.h"
#include "NodeLog.h"
#include "Normalize.h"
#include "sglobal.h"
#include "Storage.h"

CookieAuth::CookieAuth(Core *core)
  : AnonymousAuth(core)
{
}


AuthResult CookieAuth::auth(const AuthRequest &data)
{
  if (SimpleID::typeOf(data.cookie) != SimpleID::CookieId) {
    return AnonymousAuth::auth(data);
  }

  return auth(data, Ch::channel(data.cookie, SimpleID::UserId));
}


int CookieAuth::type() const
{
  return AuthRequest::Cookie;
}


AuthResult CookieAuth::auth(const AuthRequest &data, ChatChannel channel)
{
  if (!channel || isPasswordRequired(channel.data(), data.uniqueId)) {
    return AuthResult(Notice::Forbidden, data.id);
  }

  update(channel.data(), data);
  if (!channel->isValid())
    return AuthResult(Notice::BadRequest, data.id);

  Core::add(channel);
  Ch::userChannel(channel, data, m_core->packetsEvent()->address.toString());

  LOG_INFO("N2010", "Core/CookieAuth", "s:" << Core::socket() << ". " << channel->name() << "@" << m_core->packetsEvent()->address.toString() + "/" + ChatId(channel->id()).toString() << ", " << data.host)
  return AuthResult(channel->id(), data.id);
}


/*!
 * Проверка на необходимость принудительной авторизации,
 * в случае подключения зарегистрированного пользователя с нового компьютера.
 *
 * \param channel  Указатель на канал пользователя.
 * \param uniqueId Уникальный идентификатор пользователя.
 *
 * \return \b true если необходима авторизация.
 */
bool CookieAuth::isPasswordRequired(ServerChannel *channel, const QByteArray &uniqueId) const
{
  if (!channel->account() || channel->account()->provider.isEmpty())
    return false;

  if (channel->hosts()->all().contains(Hosts::toHostId(uniqueId, channel->id())))
    return false;

  return true;
}
