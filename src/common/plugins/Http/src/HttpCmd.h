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

#ifndef HTTPCMD_H_
#define HTTPCMD_H_

#include <QObject>

#include "client/ClientHooks.h"

class HttpCmd : public Hooks::Messages
{
  Q_OBJECT

public:
  HttpCmd(QObject *parent = 0);
  bool command(const QByteArray &dest, const ClientCmd &cmd) Q_DECL_OVERRIDE;
};

#endif // HTTPCMD_H
