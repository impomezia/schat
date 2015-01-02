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
#include "DataBase.h"
#include "DateTime.h"
#include "feeds/FeedsCore.h"
#include "feeds/FeedStorage.h"
#include "feeds/FeedStrings.h"
#include "net/SimpleID.h"
#include "Normalize.h"
#include "ServerChannel.h"
#include "sglobal.h"

ServerChannel::ServerChannel(ClientChannel channel)
  : Channel(channel->id(), channel->name())
  , m_permanent(false)
  , m_hosts(0)
{
  normalize();
  m_channels.set(channel->channels().all());

  if (type() == SimpleID::UserId)
    m_hosts = new Hosts(this);
}


ServerChannel::ServerChannel(const QByteArray &id, const QString &name)
  : Channel(id, name)
  , m_permanent(false)
  , m_hosts(0)
{
  normalize();

  if (type() == SimpleID::UserId)
    m_hosts = new Hosts(this);
}


ServerChannel::~ServerChannel()
{
  if (m_hosts)
    delete m_hosts;
}


/*!
 * Добавление канала в список каналов.
 */
bool ServerChannel::addChannel(const QByteArray &id)
{
  const ChatId _id(id);

  if (m_channels.add(id)) {
    m_offline.remove(id);

    if (_id.type() == ChatId::UserId) {
      FeedsCore::post(this, FEED_NAME_USERS + LC('/') + _id.toString(), Ch::server().data(), QVariant(), Feed::Broadcast);
      FeedsCore::post(this, FEED_NAME_STATS + LC('/') + _id.toString(), Ch::server().data(), QVariant(), Feed::Broadcast);
    }

    return true;
  }

  return false;
}


/*!
 * Удаление канала из списка каналов.
 */
bool ServerChannel::removeChannel(const QByteArray &id, bool offline)
{
  const ChatId _id(id);

  if (m_channels.contains(id)) {
    m_channels.remove(id);

    if (offline)
      m_offline.add(id);

    if (_id.type() == ChatId::UserId) {
      FeedsCore::del(this, FEED_NAME_USERS + LC('/') + _id.toString(), Ch::server().data(), Feed::Broadcast);
      FeedsCore::del(this, FEED_NAME_STATS + LC('/') + _id.toString(), Ch::server().data(), Feed::Broadcast);
    }

    return true;
  }

  if (!offline)
    m_offline.remove(id);

  return false;
}


/*!
 * Установка имени канала.
 */
bool ServerChannel::setName(const QString &name)
{
  if (Channel::setName(name)) {
    normalize();
    return true;
  }

  return false;
}


Hosts* ServerChannel::hosts() const
{
  Q_ASSERT(m_hosts);

  return m_hosts;
}


bool ServerChannel::canEdit(ChatChannel channel, bool special)
{
  return feed(FEED_NAME_ACL)->can(channel.data(), Acl::Edit | (special ? Acl::SpecialEdit : 0));
}


bool ServerChannel::canRead(ChatChannel channel, bool special)
{
  return feed(FEED_NAME_ACL)->can(channel.data(), Acl::Read | (special ? Acl::SpecialRead : 0));
}


bool ServerChannel::canWrite(ChatChannel channel, bool special)
{
  return feed(FEED_NAME_ACL)->can(channel.data(), Acl::Write | (special ? Acl::SpecialWrite : 0));
}


void ServerChannel::normalize()
{
  m_normalized = Normalize::toId(this);
}
