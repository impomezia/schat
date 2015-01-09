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

#include "Ch.h"
#include "cores/Core.h"
#include "DataBase.h"
#include "DateTime.h"
#include "events.h"
#include "feeds/FeedEvents.h"
#include "feeds/FeedStorage.h"
#include "Hosts.h"
#include "net/packets/auth.h"
#include "net/packets/ChannelNotice.h"
#include "net/packets/FeedNotice.h"
#include "ServerChannel.h"
#include "sglobal.h"
#include "SJMPPacket.h"
#include "Sockets.h"
#include "Storage.h"
#include "tools/Ver.h"

Hosts::Hosts(ServerChannel *channel)
  : m_date(0)
  , m_channel(channel)
{
}


const QMap<ChatId, HostInfo>& Hosts::all()
{
  if (m_hosts.isEmpty())
    m_hosts = DataBase::hosts(m_channel->key());

  return m_hosts;
}


/*!
 * Получение фида \b hosts.
 *
 * Если фид не существует, то он будет создан, при создании будет задана маска прав доступа 0400.
 */
FeedPtr Hosts::feed() const
{
  return feed(FEED_NAME_HOSTS, 0600);
}


/*!
 * Получение фида \b user.
 *
 * Если фид не существует, то он будет создан, при создании будет задана маска прав доступа 0400.
 */
FeedPtr Hosts::user() const
{
  return feed(FEED_NAME_USER, 0444);
}


QList<quint64> Hosts::sockets(const QByteArray &publicId) const
{
  if (SimpleID::typeOf(publicId) == SimpleID::HostId) {
    HostInfo host = m_hosts.value(publicId);
    return host->sockets.keys();
  }

  return QList<quint64>();
}


/*!
 * Удаление сокета.
 */
QString Hosts::remove(quint64 socket)
{
  if (socket == 0)
    socket = Core::socket();

  HostInfo host = m_sockets.value(socket);

  Q_ASSERT(host);

  if (!host)
    return QString();

  if (host->sockets.size() == 1) {
    host->online = false;
    updateHostsFeed(host, FEED_METHOD_PUT, socket);
    updateUserFeed(host, FEED_METHOD_DELETE, socket);
  }

  Q_ASSERT(!host->sockets.value(socket).isEmpty());

  if (Core::isReady()) {
    SJMPPacket packet;
    packet.setMethod(LS("delete"));
    packet.setResource(LS("token"));
    packet.setHeader(LS("user"), m_channel->nativeId());
    packet.setHeader(LS("uuid"), host->sockets.value(socket));

    Core::send(packet);
  }

  m_sockets.remove(socket);
  return host->sockets.take(socket);
}


/*!
 * Добавление нового или обновление существующего подключения в списке хостов.
 *
 * \param hostInfo Информация о хосте пользователя.
 */
void Hosts::add(HostInfo hostInfo)
{
  all();

  HostInfo host = m_hosts.value(hostInfo->hostId);

  if (host) {
    host->name    = hostInfo->name;
    host->ip      = hostInfo->ip;
    host->version = hostInfo->version;
    host->os      = hostInfo->os;
    host->osName  = hostInfo->osName;
    host->tz      = hostInfo->tz;
    host->geo     = hostInfo->geo;
  }
  else {
    host = hostInfo;
    m_hosts[hostInfo->hostId] = host;
  }

  host->online  = true;
  host->channel = m_channel->key();

  host->sockets.insert(hostInfo->socket, hostInfo->uuid);
  m_sockets[hostInfo->socket] = host;

  updateHostsFeed(host, FEED_METHOD_POST, hostInfo->socket);
  updateUserFeed(host, FEED_METHOD_POST, hostInfo->socket);
}


void Hosts::unlink(const QByteArray &hostId)
{
  HostInfo host = m_hosts.value(hostId);
  Q_ASSERT(host);
  if (!host)
    return;

  SJMPPacket packet;
  packet.setMethod(LS("delete"));
  packet.setResource(LS("v4/session/") + host->nativeId);

  if (host->online)
    Core::sendSync(packet);
  else
    Core::send(packet);

  DataBase::removeHost(host->nativeId.toLatin1());

  m_date = DateTime::utc();
  updateUserFeed(host, FEED_METHOD_DELETE, 0);

  QList<quint64> sockets = host->sockets.keys();
  if (!sockets.isEmpty()) {
    Core::i()->send(sockets, ChannelNotice::request(m_channel->id(), m_channel->id(), LS("quit"))->data(Core::stream()), NewPacketsEvent::KillSocketOption);
  }
}


/*!
 * Служебная функция получения фида.
 *
 * На фид установляется маска прав доступа \p mask и если фид не существует добавляется владелец фида.
 */
FeedPtr Hosts::feed(const QString &name, int mask) const
{
  FeedPtr feed = m_channel->feed(name, false);
  if (!feed) {
    feed = m_channel->feed(name, true, false);
    feed->head().acl().add(m_channel->id());
  }

  feed->head().acl().setMask(mask);
  return feed;
}


/*!
 * Фиксация события обновления фида \b hosts и отправка уведомления об этом.
 *
 * \param host   Информация о подключении.
 * \param method \b post если пользователь подключился или \b put если отключился.
 * \param socket Номер сокета подключения.
 */
void Hosts::updateHostsFeed(HostInfo host, const QString &method, quint64 socket)
{
  m_date           = DateTime::utc();
  host->date       = m_date;

  FeedPtr hosts    = feed();
  FeedEvent *event = new FeedEvent(m_channel->id(), m_channel->id(), method);
  event->name      = FEED_NAME_HOSTS;
  event->diffTo    = hosts->head().date();
  event->date      = m_date;
  event->status    = Notice::OK;
  event->path      = host->hostId.toBase32();
  event->socket    = socket;

  DataBase::add(host);
  FeedStorage::save(hosts, m_date);
  FeedEvents::start(event);
}


/*!
 * Фиксация события обновления фида \b user и отправка уведомления об этом.
 */
void Hosts::updateUserFeed(HostInfo host, const QString &method, quint64 socket)
{
  FeedPtr user     = this->user();
  FeedEvent *event = new FeedEvent(m_channel->id(), m_channel->id(), method);

  if (!user->head().f().isEmpty()) {
    event->broadcast = Sockets::all(Ch::channel(m_channel->id()), true);

    if (method == FEED_METHOD_POST)
      event->broadcast.removeAll(socket);
  }

  event->name      = FEED_NAME_USER;
  event->diffTo    = user->head().date();
  event->date      = m_date;
  event->status    = Notice::OK;
  event->path      = host->hostId.toBase32();
  event->socket    = socket;

  user->data()[LS("last")] = event->path;

  FeedStorage::save(user, m_date);
  FeedEvents::start(event);
}
