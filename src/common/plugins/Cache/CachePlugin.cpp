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

#include <QtPlugin>
#include <QFile>

#include "CacheChannels.h"
#include "CacheDB.h"
#include "CachePlugin.h"
#include "CachePlugin_p.h"
#include "ChatCore.h"
#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/SimpleClient.h"
#include "feeds/CacheFeedStorage.h"
#include "JSON.h"
#include "net/dns/ChatDNS.h"
#include "net/SimpleID.h"
#include "NetworkManager.h"
#include "Path.h"
#include "sglobal.h"
#include "StateCache.h"
#include "TalksCache.h"
#include "Translation.h"

Cache::Cache(QObject *parent)
  : ChatPlugin(parent)
{
  new CacheDB(this);
  new CacheFeedStorage(this);
  new TalksCache(this);
  new StateCache(this);

  connect(ChatClient::i(), SIGNAL(online()), SLOT(open()));
  connect(ChatClient::i(), SIGNAL(ready()), SLOT(onClientReady()));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(onNotify(Notify)));

  ChatCore::translation()->addOther(LS("cache"));

  if (!ChatCore::isReady())
    connect(ChatCore::i(), SIGNAL(ready()), SLOT(onChatReady()));
  else
    onChatReady();
}


void Cache::onChatReady()
{
  new Hooks::CacheChannels(this);

  loadCache();
  open();
}


void Cache::onClientReady()
{
  QFile file(Path::cache() + LS("/dns.cache"));
  if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    file.write(JSON::generate(ChatClient::io()->dns()->cache()));
}


void Cache::onNotify(const Notify &notify)
{
  if (notify.type() == Notify::ClearCache) {
    CacheDB::clear();

    foreach (ClientChannel channel, ChatClient::channels()->channels()) {
      channel->setKey(0);
      channel->data().clear();
    }

    CacheDB::add(ChatClient::server());
    CacheDB::add(ChatClient::channel());

    ChatClient::io()->dns()->setCache(QVariantMap());
    onClientReady();
  }
}


void Cache::open()
{
  const QByteArray id = ChatClient::serverId();
  if (id.isEmpty())
    return;

  if (!CacheDB::open(id, ChatCore::networks()->root(SimpleID::encode(id))))
    return;

  if (ChatClient::serverId().isEmpty())
    return;

  load(ChatClient::server());
  load(ChatClient::channel());
}


/*!
 * Загрузка фидов для собственного канала и канала сервера.
 *
 * Функция вызывается сразу после подключения к серверу или при запуске клиента.
 * Если идентификатор сервера пустой, то считается что это серверный канал.
 */
void Cache::load(ClientChannel channel)
{
  const QByteArray id = channel->id();
  if (channel->id().isEmpty())
    channel->setId(ChatClient::serverId());

  ClientChannel exist = CacheDB::channel(channel->id(), false);
  if (!exist) {
    if (id.isEmpty())
      channel->resetId();

    return;
  }

  channel->setData(exist->data());
  FeedStorage::load(channel.data());

  if (id.isEmpty())
    channel->resetId();
}


void Cache::loadCache()
{
  QFile file(Path::cache() + LS("/dns.cache"));
  if (file.open(QIODevice::ReadOnly)) {
    const QVariantMap data = JSON::parse(file.readAll()).toMap();
    ChatClient::io()->dns()->setCache(data);
  }
}


ChatPlugin *CachePlugin::create()
{
  m_plugin = new Cache(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(Cache, CachePlugin);
#endif
