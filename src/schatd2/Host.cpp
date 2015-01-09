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

#include "Host.h"
#include "net/packets/auth.h"
#include "sglobal.h"
#include "SJMPPacket.h"

Host::Host(const AuthRequest &data, const SJMPPacket &packet, const QString &ip, const QString &uuid, quint64 socket)
  : online(true)
  , hostId(ChatId(packet.header(LS("session")).toByteArray(), ChatId::HostId))
  , nativeId(packet.header(LS("session")).toString())
  , channel(0)
  , name(data.hostName)
  , ip(ip)
  , version(data.version)
  , os(data.os)
  , osName(data.json.value(LS("os")).toString())
  , tz(data.tz)
  , geo(packet.body().toMap().value(LS("geo")).toMap())
  , uniqueId(data.uniqueId)
  , socket(socket)
  , uuid(uuid)
{

}
