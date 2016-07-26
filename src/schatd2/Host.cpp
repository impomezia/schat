/* Simple Chat
 * Copyright (c) 2008-2016 Alexander Sedov <imp@schat.me>
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

#include "Host.h"
#include "net/packets/auth.h"
#include "sglobal.h"


Host::Host(const AuthRequest &data, const QString &address, quint64 socket)
  : id(0)
  , online(true)
  , channel(0)
  , name(data.hostName)
  , address(address)
  , version(data.version)
  , os(data.os)
  , osName(data.json.value(LS("os")).toString())
  , tz(data.tz)
  , uniqueId(data.uniqueId)
  , socket(socket)
{
}


QDebug operator<<(QDebug dbg, const Host &host) {
  dbg.nospace() << "Host<" << host.id << "/" << host.channel << "> "
                << host.address.toUtf8().constData() << " -- "
                << '"' << host.name.toUtf8().constData() << "/" << host.osName.toUtf8().constData() << '"';

  return dbg.nospace();
}
