/* $Id: DiscoveryAuth.h 3467 2013-02-05 03:17:57Z IMPOMEZIA $
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

#ifndef DISCOVERYAUTH_H_
#define DISCOVERYAUTH_H_

#include "cores/AnonymousAuth.h"

class DiscoveryAuth : public AnonymousAuth
{
public:
  DiscoveryAuth(Core *core);
  AuthResult auth(const AuthRequest &data);
  int type() const;
};

#endif /* DISCOVERYAUTH_H_ */
