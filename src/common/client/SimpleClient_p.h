/* $Id: SimpleClient_p.h 3497 2013-02-12 07:37:32Z IMPOMEZIA $
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

#ifndef SIMPLECLIENT_P_H_
#define SIMPLECLIENT_P_H_

#include "client/AbstractClient_p.h"
#include "client/SimpleClient.h"

class PacketReader;
class SyncChannelCache;
class UserReader;

class SimpleClientPrivate : public AbstractClientPrivate
{
  Q_DECLARE_PUBLIC(SimpleClient);

public:
  SimpleClientPrivate();
  virtual ~SimpleClientPrivate();

  AuthReplyAction authReply(const AuthReply &reply);
  bool notice();

  int authType;    ///< Тип авторизации.
};

#endif /* SIMPLECLIENT_P_H_ */
