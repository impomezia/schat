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

#ifndef HTTPERROR_H_
#define HTTPERROR_H_

#include "interfaces/INetworkError.h"
#include "schat.h"

class QNetworkReply;

class HttpError : public INetworkError
{
public:
  HttpError(QNetworkReply *reply, int error = -1);
  inline int error() const  Q_DECL_OVERRIDE { return m_error; }
  inline int status() const Q_DECL_OVERRIDE { return m_status; }
  static HttpError *create(QNetworkReply *reply);

private:
  int m_error;
  int m_status;
};

#endif // HTTPERROR_H_
