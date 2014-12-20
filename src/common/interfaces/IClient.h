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

#ifndef ICLIENT_H_
#define ICLIENT_H_

class IClientListener;
class SJMPPacket;

class IClient
{
public:
  virtual ~IClient() {}
  virtual void leave(int status = 1001) = 0;
  virtual void open(const QString &hostName, quint16 port = 7665) = 0;
  virtual void ping() = 0;
  virtual void reconnect() = 0;
  virtual void send(const SJMPPacket &packet) = 0;

  virtual void addListener(IClientListener *listener) = 0;
  virtual void removeListener(IClientListener *listener) = 0;
};

#endif // ICLIENT_H_
