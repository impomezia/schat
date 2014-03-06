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

#include "ChatCore.h"
#include "client/ChatClient.h"
#include "client/ClientCmd.h"
#include "client/ClientMessages.h"
#include "HttpCmd.h"
#include "NetworkAccess.h"
#include "sglobal.h"

HttpCmd::HttpCmd(QObject *parent)
  : Hooks::Messages(parent)
{
  ChatClient::messages()->add(this);
}


bool HttpCmd::command(const QByteArray &dest, const ClientCmd &cmd)
{
  Q_UNUSED(dest);

  const QString command = cmd.command().toLower();

  if (command == LS("wget")) {
    if (cmd.body().isEmpty())
      return true;

    ClientCmd body(cmd.body());
    ChatCore::networkAccess()->download(body.command(), body.body());
    return true;
  }

  return false;
}
