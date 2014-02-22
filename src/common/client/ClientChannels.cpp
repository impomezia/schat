/* $Id: ClientChannels.cpp 3724 2013-07-02 23:42:52Z IMPOMEZIA $
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
#include "client/ClientHooks.h"
#include "client/SimpleClient.h"
#include "debugstream.h"
#include "feeds/ServerFeed.h"
#include "net/Channels.h"
#include "net/packets/ChannelNotice.h"
#include "net/SimpleID.h"
#include "sglobal.h"

ClientChannels::ClientChannels(QObject *parent)
  : QObject(parent)
  , m_policy(ServerFeed::MainChannelPolicy | ServerFeed::ForcedJoinPolicy)
  , m_client(ChatClient::io())
{
  m_hooks = new Hooks::Channels(this);

  connect(m_client, SIGNAL(notice(int)), SLOT(notice(int)));
  connect(m_client, SIGNAL(idle()), SLOT(idle()));
  connect(m_client, SIGNAL(restore()), SLOT(restore()));
  connect(m_client, SIGNAL(setup()), SLOT(setup()));
  connect(m_client, SIGNAL(clientStateChanged(int,int)), SLOT(clientStateChanged(int,int)));
}


/*!
 * Получение канал по идентификатору.
 *
 * \param id Идентификатор канала.
 * \return Канал или пустой канал, если поиск неудачен.
 */
ClientChannel ClientChannels::get(const QByteArray &id)
{
  if (id.isEmpty())
    return ClientChannel();

  if (!m_channels.contains(id)) {
    if (ChatClient::id() == id)
      m_channels[id] = ChatClient::channel();
    else if (ChatClient::serverId() == id)
      m_channels[id] = ChatClient::server();
  }

  ClientChannel channel = m_channels.value(id);
  if (!channel || channel->id() != id) {
    channel = m_hooks->get(id);

    if (channel)
      m_channels[id] = channel;
  }

  return channel;
}


/*!
 * Вход в канал по идентификатору.
 *
 * \param id Идентификатор канала.
 */
bool ClientChannels::join(const QByteArray &id)
{
  if (!Channel::isCompatibleId(id))
    return false;

  if (m_client->clientState() != SimpleClient::ClientOnline) {
    m_joined.append(id);
    return false;
  }

  return m_client->send(ChannelNotice::request(ChatClient::id(), id, CHANNELS_JOIN_CMD));
}


/*!
 * Подключение к обычному каналу по имени.
 *
 * \param name Имя канала.
 */
bool ClientChannels::join(const QString &name)
{
  if (!Channel::isValidName(name))
    return false;

  return m_client->send(ChannelNotice::request(ChatClient::id(), QByteArray(), CHANNELS_JOIN_CMD, name));
}


bool ClientChannels::name(const QByteArray &id, const QString &name)
{
  if ((SimpleID::typeOf(id) == SimpleID::ServerId && name.isEmpty()) || Channel::isValidName(name))
    return m_client->send(ChannelNotice::request(ChatClient::id(), id, CHANNELS_NAME_CMD, name));

  return false;
}


bool ClientChannels::nick(const QString &nick)
{
  ClientChannel user(new Channel(ChatClient::channel()->id(), nick));
  user->gender() = ChatClient::channel()->gender().raw();
  user->status() = ChatClient::channel()->status().value();

  if (!user->isValid())
    return false;

  return m_client->send(ChannelNotice::update(user));
}


/*!
 * Отключение от канала.
 */
bool ClientChannels::part(const QByteArray &id)
{
  if (!Channel::isCompatibleId(id))
    return false;

  m_joined.removeAll(id);

  return m_client->send(ChannelNotice::request(ChatClient::id(), id, CHANNELS_PART_CMD));
}


bool ClientChannels::sync(const QList<QByteArray> &channels)
{
  QList<QByteArray> ids;
  foreach (const QByteArray &id, channels) {
    if (!m_unsynced.contains(id) && !ids.contains(id)) {
      m_unsynced.append(id);
      ids.append(id);
    }
  }

  return info(ids);
}


bool ClientChannels::update()
{
  return m_client->send(ChannelNotice::update(ChatClient::channel()));
}


void ClientChannels::clientStateChanged(int state, int previousState)
{
  Q_UNUSED(state)

  if (previousState == ChatClient::Online) {
    m_joined.removeAll(ChatClient::id());

    foreach (ClientChannel channel, m_channels) {
      channel->setSynced(false);
      channel->channels().clear();
    }
  }
  else if (state == ChatClient::Online) {
    m_mainId = m_client->json().value(CLIENT_PROP_CHANNEL).toByteArray();
    m_policy = m_client->json().value(CLIENT_PROP_POLICY).toInt();
  }
}


/*!
 * Слот вызывается, когда клиент прочитает все пришедшие пакеты.
 */
void ClientChannels::idle()
{
  if (!m_synced.isEmpty()) {
    emit channels(m_synced);
    m_synced.clear();
  }
}


/*!
 * Обработка получения нового уведомления.
 *
 * \param type Тип уведомления, должен быть равен Notice::ChannelType.
 */
void ClientChannels::notice(int type)
{
  if (type != Notice::ChannelType)
    return;

  ChannelNotice packet(type, ChatClient::io()->reader());
  if (!packet.isValid())
    return;

  m_packet = &packet;
  const QString cmd = m_packet->command();
  SCHAT_DEBUG_STREAM(this << "notice()" << cmd << m_packet->text() << m_packet->status() << Notice::status(m_packet->status()));
# if defined(SCHAT_DEBUG)
  qDebug() << "             JSON SIZE:" << m_packet->raw().size() << "bytes";
  qDebug() << "             JSON DATA:" << m_packet->raw();
# endif

  if (cmd == CHANNELS_CHANNEL_CMD)
    channel();

  else if (cmd == CHANNELS_INFO_CMD)
    add();

  else if (cmd == CHANNELS_JOINED_CMD)
    join();

  else if (cmd == CHANNELS_PART_CMD)
    part();

  else if (cmd == CHANNELS_QUIT_CMD)
    quit();

  emit notice(packet);
}


void ClientChannels::restore()
{
  QList<QByteArray> channels = m_channels.keys();
  m_client->lock();

  foreach (const QByteArray &id, m_joined) {
    channels.removeAll(id);
    join(id);
  }

  channels.removeAll(m_client->channelId());
  sync(channels);
  m_client->unlock();
}


void ClientChannels::setup()
{
  m_channels.clear();
  m_synced.clear();
  m_unsynced.clear();
  m_joined.clear();
}


/*!
 * Запрос информации о каналах.
 *
 * \param channels Список идентификаторов каналов.
 */
bool ClientChannels::info(const QList<QByteArray> &channels)
{
  if (channels.isEmpty())
    return false;

  if (channels.size() > 256) {
    bool result = false;
    int size = channels.size();
    int pos = 0;

    do {
      result = m_client->send(ChannelNotice::info(ChatClient::id(), channels.mid(pos, 256)));
      if (!result)
        return result;

      pos += 256;
    } while (pos < size);

    return true;
  }

  return m_client->send(ChannelNotice::info(ChatClient::id(), channels));
}


/*!
 * Добавление канала в таблицу каналов.
 */
ClientChannel ClientChannels::add()
{
  const QByteArray &id = m_packet->channelId;
  if (!Channel::isCompatibleId(id))
    return ClientChannel();

  m_unsynced.removeAll(id);
  ClientChannel channel = get(id);
  ChannelInfo info(id);

  if (!channel) {
    channel = ClientChannel(new Channel(id, m_packet->text()));
    m_channels[id] = channel;
  }
  else
    info.setOption(ChannelInfo::Updated);

  if (m_packet->text() == FEED_WILDCARD_ASTERISK) {
    if (channel->setName(ChatClient::io()->url().host()))
      info.setOption(ChannelInfo::Renamed);
  }
  else {
    const QString name = channel->name();
    if (channel->setName(m_packet->text()) && channel->name() != name)
      info.setOption(ChannelInfo::Renamed);
  }

  channel->gender() = m_packet->gender;
  channel->status() = m_packet->channelStatus;

  if (channel->type() == SimpleID::ChannelId && m_packet->status() == Notice::Forbidden) {
    channel->gender().setColor(Gender::Green);
    channel->feeds().remove(FEED_NAME_ACL);
  }

  m_synced += channel->id();
  m_hooks->add(channel, info, m_packet->json());
  emit this->channel(info);

  return channel;
}


/*!
 * Чтение заголовка канала.
 */
void ClientChannels::channel()
{
  ClientChannel channel = add();

  if (!m_joined.contains(channel->id()))
    m_joined += channel->id();

  if (ChatClient::id() == channel->id()) {
    foreach (const QByteArray &id, m_packet->channels) {
      if (!m_joined.contains(id))
        m_joined.append(id);
    }
  }

  emit this->channel(channel->id());

  if (channel->type() == SimpleID::ChannelId) {
    channel->channels() = m_packet->channels;
    sync(channel);
  }
}


/*!
 * Обработка входа пользователя в канал.
 */
void ClientChannels::join()
{
  ClientChannel user = add();
  ClientChannel channel = get(m_packet->dest());

  channel->channels() += user->id();

  emit joined(channel->id(), user->id());
}


void ClientChannels::part()
{
  ClientChannel user = get(m_packet->sender());
  if (!user)
    return;

  ClientChannel channel = get(m_packet->dest());
  if (!channel)
    return;

  emit part(channel->id(), user->id());

  channel->channels().remove(user->id());
}


void ClientChannels::quit()
{
  ClientChannel user = get(m_packet->sender());
  if (!user)
    return;

  const bool offline = ChatClient::server()->feed(FEED_NAME_SERVER)->data().value(SERVER_FEED_OFFLINE_KEY, true).toBool();
  user->setSynced(false);
  user->status() = Status::Offline;
  emit quit(user->id(), offline);

  QMapIterator<QByteArray, ClientChannel> i(m_channels);
  while (i.hasNext()) {
    i.next();
    i.value()->channels().remove(user->id());
  }
}


/*!
 * Формирование запроса информации об не известных каналах.
 *
 * \param channel Указатель на канал.
 */
void ClientChannels::sync(ClientChannel channel)
{
  if (channel->type() != SimpleID::ChannelId)
    return;

  const QList<QByteArray>& channels = channel->channels().all();
  QList<QByteArray> unsynced;

  foreach (const QByteArray &id, channels) {
    ClientChannel exist = get(id);
    if (exist && exist->isSynced()) {
      if (!m_synced.contains(id))
        m_synced += id;
    }
    else if (!m_unsynced.contains(id))
      unsynced += id;
  }

  m_unsynced.append(unsynced);
  info(unsynced);
}
