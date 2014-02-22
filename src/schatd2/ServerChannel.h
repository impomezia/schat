/* $Id: ServerChannel.h 3639 2013-04-16 08:30:13Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#ifndef SERVERCHANNEL_H_
#define SERVERCHANNEL_H_

#include "Channel.h"
#include "Hosts.h"

class ServerChannel;
class User;

typedef QSharedPointer<ServerChannel> ChatChannel;

/*!
 * Серверный канал.
 */
class SCHAT_EXPORT ServerChannel: public Channel
{
public:
  ServerChannel(ClientChannel channel);
  ServerChannel(const QByteArray &id, const QString &name);
  ~ServerChannel();

  inline bool permanent() const                { return m_permanent; }
  inline Channels& channels()                  { return m_channels; }
  inline Channels& offline()                   { return m_offline; }
  inline const Channels& channels() const      { return m_channels; }
  inline const Channels& offline() const       { return m_offline; }
  inline const QByteArray& normalized() const  { return m_normalized; }
  inline Hosts* hosts() const                  { return m_hosts; }
  inline QList<quint64> sockets() const        { if (m_hosts) return m_hosts->sockets(); return QList<quint64>(); }
  inline User* user() const                    { return m_user; }
  inline void setPermanent(bool permanent)     { m_permanent = permanent; }

  bool addChannel(const QByteArray &id);
  bool removeChannel(const QByteArray &id, bool offline = false);
  bool setName(const QString &name);
  void createAccount();
  void setKey(qint64 key);

  bool canEdit(ChatChannel channel, bool special = true);
  bool canRead(ChatChannel channel, bool special = false);
  bool canWrite(ChatChannel channel, bool special = false);

private:
  void normalize();

  bool m_permanent;         ///< \b true если канал не будет удалятся если он не нужен.
  Channels m_offline;       ///< Список офлайн каналов.
  Hosts *m_hosts;           ///< Информация о хостах.
  QByteArray m_normalized;  ///< Нормализованное имя канала.
  User *m_user;             ///< Профиль пользователя.
};

#endif /* SERVERCHANNEL_H_ */
