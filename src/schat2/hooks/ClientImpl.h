/* $Id: ClientImpl.h 3664 2013-05-11 18:19:01Z IMPOMEZIA $
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

#ifndef CLIENTIMPL_H_
#define CLIENTIMPL_H_

#include "client/ClientHooks.h"

namespace Hooks
{

class ClientImpl : public Client
{
  Q_OBJECT

public:
  ClientImpl(QObject *parent = 0);

  void setup();

private slots:
  void channel(const QByteArray &id);

private:
  bool m_setup; ///< \b true если необходимо выполнить работы по установке соединения после того как получен профиль пользователя.
};

} // namespace Hooks

#endif /* CLIENTIMPL_H_ */
