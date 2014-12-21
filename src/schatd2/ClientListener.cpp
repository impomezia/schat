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

#include "ClientListener.h"
#include "SJMPPacket.h"
#include "interfaces/IClient.h"

ClientListener::ClientListener(const QString &token, QObject *parent)
  : QObject(parent)
  , m_waitAuth(true)
  , m_token(token)
{
}


void ClientListener::onAuthRequired(IClient *client)
{
  Q_ASSERT(client);

  m_waitAuth = true;

  SJMPPacket packet;
  packet.setType(SJMPPacket::Request);
  packet.setResource(QLatin1String("token"));
  packet.setHeader(QLatin1String("token"), m_token);

  client->send(packet, false);
}


void ClientListener::onConnectToHost(IClient *client, const QString &hostName, quint16 port)
{
  Q_UNUSED(client)
  Q_UNUSED(hostName)
  Q_UNUSED(port)
}


void ClientListener::onDisconnected(IClient *client, int status)
{
  Q_UNUSED(client)
  Q_UNUSED(status)
}


void ClientListener::onPacket(IClient *client, const SJMPPacket &packet)
{
  if (m_waitAuth) {
    m_waitAuth = false;

    client->setReady(packet.status() == 200);
  }
}


void ClientListener::onPong(IClient *client)
{
  Q_UNUSED(client)
}
