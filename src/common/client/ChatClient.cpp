/* $Id: ChatClient.cpp 3776 2013-08-24 06:03:44Z IMPOMEZIA $
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

#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientFeeds.h"
#include "client/ClientHooks.h"
#include "client/ClientMessages.h"
#include "client/SimpleClient.h"
#include "DateTime.h"
#include "net/SimpleID.h"
#include "sglobal.h"

ChatClient *ChatClient::m_self = 0;

ChatClient::ChatClient(QObject *parent)
  : QObject(parent)
{
  m_self = this;

  m_client = new SimpleClient(this);

  m_channels = new ClientChannels(this);
  m_messages = new ClientMessages(this);
  m_feeds = new ClientFeeds(this);

  m_hooks = new Hooks::Client(this);

  connect(m_client, SIGNAL(restore()), SLOT(restore()));
  connect(m_client, SIGNAL(setup()), SLOT(setup()));
  connect(m_client, SIGNAL(ready()), SIGNAL(ready()));
  connect(m_client, SIGNAL(clientStateChanged(int,int)), SLOT(clientStateChanged(int,int)));
}


ClientChannel ChatClient::channel()
{
  return io()->channel();
}


ClientChannel ChatClient::server()
{
  return io()->server();
}


int ChatClient::state()
{
  return io()->clientState();
}


/*!
 * Получение идентификатора пользователя.
 *
 * \sa Hooks::Client::id()
 */
QByteArray ChatClient::id()
{
  ChatId id(io()->channelId());

  if (!id.isNull())
    return id.toByteArray();

  id.init(io()->channel()->id());
  if (!id.isNull())
    return id.toByteArray();

  return m_self->m_hooks->id();
}


QByteArray ChatClient::serverId()
{
  ChatId id(io()->server()->id());
  if (!id.isNull())
    return id.toByteArray();

  return m_self->m_hooks->serverId();
}


QDataStream *ChatClient::stream()
{
  return io()->sendStream();
}


/*!
 * Получение отметки времени.
 */
qint64 ChatClient::date()
{
  qint64 out = io()->date();
  if (!out)
    out = DateTime::utc();

  return out;
}


/*!
 * Получение имени сервера.
 */
QString ChatClient::serverName()
{
  return io()->serverName();
}


bool ChatClient::open(const QByteArray &id)
{
  bool matched = false;
  return m_self->m_hooks->openId(id, matched);
}


bool ChatClient::open(const QUrl &url)
{
  return io()->openUrl(url);
}


void ChatClient::clientStateChanged(int state, int previousState)
{
  if (previousState == Online)
    emit offline();

  if (state == Online) {
    if (!m_id.isEmpty() && m_id != serverId()) {
      server()->data().clear();
      server()->feeds().clear();
      channel()->data().clear();
      channel()->feeds().clear();
    }

    m_id = serverId();
    emit online();
  }
}


void ChatClient::restore()
{
  m_hooks->restore();
}


void ChatClient::setup()
{
  m_hooks->setup();
}


ChatClientLocker::ChatClientLocker()
{
  m_client = ChatClient::io();
  m_client->lock();
}


ChatClientLocker::ChatClientLocker(SimpleClient *client)
  : m_client(client)
{
  m_client->lock();
}


ChatClientLocker::~ChatClientLocker()
{
  m_client->unlock();
}


void ChatClientLocker::unlock()
{
  m_client->unlock();
}
