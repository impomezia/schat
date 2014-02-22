/* $Id: events.cpp 2133 2012-01-12 05:55:51Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#include "events.h"

ServerEvent::ServerEvent(ServerEvents type, const QList<quint64> &sockets, const QByteArray &channelId)
  : QEvent(static_cast<QEvent::Type>(type))
  , m_channelId(channelId)
  , m_sockets(sockets)
{
}


ServerEvent::ServerEvent(ServerEvents type, quint64 socket, const QByteArray &channelId)
  : QEvent(static_cast<QEvent::Type>(type))
  , m_channelId(channelId)
{
  m_sockets.append(socket);
}


/*!
 * Получение первого сокета в списке.
 */
quint64 ServerEvent::socket() const
{
  if (m_sockets.isEmpty())
    return 0;

  return m_sockets.at(0);
}


NewPacketsEvent::NewPacketsEvent(const QList<quint64> &sockets, const QByteArray &packet, const QByteArray &userId)
  : ServerEvent(NewPackets, sockets, userId)
  , packets(QList<QByteArray>() << packet)
  , option(0)
  , timestamp(0)
{
}


NewPacketsEvent::NewPacketsEvent(const QList<quint64> &sockets, const QList<QByteArray> &packets, const QByteArray &userId)
  : ServerEvent(NewPackets, sockets, userId)
  , packets(packets)
  , option(0)
  , timestamp(0)
{
}


SocketReleaseEvent::SocketReleaseEvent(quint64 socket, const QString &errorString, const QByteArray &userId)
  : ServerEvent(SocketRelease, socket, userId)
  , errorString(errorString)
{
}


NewConnectionEvent::NewConnectionEvent(int socketDescriptor, quint64 socket)
  : ServerEvent(NewConnection, socket)
  , socketDescriptor(socketDescriptor)
{
}
