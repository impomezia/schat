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

#ifndef NODEAUTHREPLY_H_
#define NODEAUTHREPLY_H_

#include "net/packets/auth.h"
#include "ServerChannel.h"

class AuthResult;

class NodeAuthReply : public AuthReply
{
public:
  NodeAuthReply(const AuthRequest &request, const AuthResult &result, ChatChannel user = ChatChannel());

private:
  QString path(const QUrl &url) const;
};

#endif /* NODEAUTHREPLY_H_ */
