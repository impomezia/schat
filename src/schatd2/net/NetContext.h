/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

#ifndef NETCONTEXT_H_
#define NETCONTEXT_H_

#include <QString>

#include "schat.h"

class NetRequest;

class SCHAT_EXPORT NetContext
{
public:
  NetContext(NetRequest *req, qint64 socket);
  ~NetContext();

  inline const NetRequest *req() const { return m_req; }
  inline qint64 date() const           { return m_date; }
  inline qint64 socket() const         { return m_socket; }

private:
  const NetRequest *m_req; ///< Указатель на запрос.
  const qint64 m_date;     ///< Дата получения запроса.
  const qint64 m_socket;   ///< Сокет запроса.
};

#endif /* NETCONTEXT_H_ */
