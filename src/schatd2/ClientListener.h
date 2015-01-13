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

#ifndef CLIENTLISTENER_H_
#define CLIENTLISTENER_H_

#include <QObject>

#include "interfaces/IClientListener.h"
#include "schat.h"

class ClientListener : public QObject, public IClientListener
{
  Q_OBJECT

public:
  ClientListener(const QString &token, QObject *parent = 0);
  void onAuthRequired(IClient *client) Q_DECL_OVERRIDE;
  void onConnectToHost(IClient *client, const QString &hostName, quint16 port) Q_DECL_OVERRIDE;
  void onDisconnected(IClient *client, int status) Q_DECL_OVERRIDE;
  void onPacket(IClient *client, const SJMPPacket &packet) Q_DECL_OVERRIDE;
  void onPong(IClient *client) Q_DECL_OVERRIDE;

signals:
  void packet(const SJMPPacket &packet);
  void rejoin();

private:
  bool m_waitAuth;
  QString m_token;
};

#endif // CLIENTLISTENER_H_
