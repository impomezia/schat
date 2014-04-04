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

#include "net/TcpServer.h"

TcpServer::TcpServer(QObject *parent)
  : QTcpServer(parent)
{
}


bool TcpServer::listen(const QString &host)
{
  const int index = host.lastIndexOf(QLatin1String(":"));
  if (index == -1)
    return false;

  const QHostAddress address = QHostAddress(host.left(index));
  if (address.isNull())
    return false;

  const quint16 port = host.mid(index + 1).toUInt();
  if (!port)
    return false;

  return QTcpServer::listen(address, port);
}


void TcpServer::incomingConnection(int socketDescriptor)
{
  emit newConnection(socketDescriptor);
}
