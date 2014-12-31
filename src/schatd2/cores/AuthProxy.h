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

#ifndef AUTHPROXY_H_
#define AUTHPROXY_H_

#include "net/packets/auth.h"

class AuthRequest;
class SJMPPacket;

class AuthProxy : public QObject
{
  Q_OBJECT

public:
  AuthProxy(const AuthRequest &data, const QString &ip, QObject *parent = 0);

private slots:
  void onPacket(const SJMPPacket &packet);

private:
  const AuthRequest m_data;
  const QString m_id;
  const QString m_ip;
  const QString m_uuid;
  const quint64 m_socket;
};

#endif // AUTHPROXY_H_
