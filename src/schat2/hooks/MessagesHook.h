/* $Id: MessagesHook.h 3485 2013-02-10 09:59:44Z IMPOMEZIA $
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

#ifndef MESSAGESHOOK_H_
#define MESSAGESHOOK_H_

#include "client/ClientHooks.h"

class SCHAT_CORE_EXPORT MessagesHook : public Hooks::Messages
{
  Q_OBJECT

public:
  MessagesHook(QObject *parent = 0);

protected:
  bool isOnline() const;
  bool isTalk(const QByteArray &id, const QString &command) const;
};

#endif /* MESSAGESHOOK_H_ */
