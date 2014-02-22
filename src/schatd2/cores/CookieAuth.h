/* $Id: CookieAuth.h 2916 2012-07-24 21:30:27Z IMPOMEZIA $
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

#ifndef COOKIEAUTH_H_
#define COOKIEAUTH_H_

#include "cores/AnonymousAuth.h"
#include "ServerChannel.h"

class SCHAT_EXPORT CookieAuth : public AnonymousAuth
{
public:
  CookieAuth(Core *core);
  AuthResult auth(const AuthRequest &data);
  int type() const;

protected:
  AuthResult auth(const AuthRequest &data, ChatChannel channel);
  bool isPasswordRequired(ServerChannel *channel, const QByteArray &uniqueId) const;
};

#endif /* COOKIEAUTH_H_ */
