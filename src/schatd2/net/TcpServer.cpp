/* $Id: TcpServer.cpp 2137 2012-01-12 14:05:41Z IMPOMEZIA $
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

#include <QDebug>
#include <QThread>

#include "net/TcpServer.h"

TcpServer::TcpServer(QObject *parent)
  : QTcpServer(parent)
{
}


bool TcpServer::listen(const QString &host)
{
  int index = host.lastIndexOf(QLatin1String(":"));
  if (index == -1)
    return false;

  QHostAddress address = QHostAddress(host.left(index));
  if (address.isNull())
    return false;

  quint16 port = host.mid(index + 1).toUInt();
  if (!port)
    return false;

  return QTcpServer::listen(address, port);
}


void TcpServer::incomingConnection(int socketDescriptor)
{
  emit newConnection(socketDescriptor);
}
