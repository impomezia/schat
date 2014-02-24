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

#include <QTimer>

#include "Ch.h"
#include "cores/Core.h"
#include "DataBase.h"
#include "DateTime.h"
#include "events.h"
#include "feeds/ChannelFeed.h"
#include "feeds/FeedEvents.h"
#include "feeds/FeedsCore.h"
#include "feeds/NodeChannelFeed.h"
#include "feeds/ServerFeed.h"
#include "net/Channels.h"
#include "net/PacketReader.h"
#include "NodeChannels.h"
#include "NodeLog.h"
#include "Normalize.h"
#include "sglobal.h"
#include "Sockets.h"
#include "Storage.h"

#define LOG_TAG "GenericNode/Channels"
#define LOG_G1010  LOG_INFO("G1010", LOG_TAG, "s:" << Core::socket() << ". Channel request:" << " id:"  << ChatId(m_user->id()).toString() << ", cid:" << ChatId(m_packet->channelId).toString() << ", cmd:" << cmd << ", txt:" << m_packet->text() << ", usr:" << m_user->name() )
#define LOG_G1011 LOG_DEBUG("G1011", LOG_TAG, "s:" << Core::socket() << ". info: count:" << m_packet->channels.size())
#define LOG_G1012 LOG_TRACE("G1012", LOG_TAG, "s:" << Core::socket() << ". info: id:" << ChatId(channel->id()).toString() << ", name:" << channel->name())
#define LOG_G1013 LOG_DEBUG("G1013", LOG_TAG, "s:" << Core::socket() << ". join: id:" << channelId.toString() << ", name:" << name)
#define LOG_G1014 LOG_ERROR("G1014", LOG_TAG, "s:" << Core::socket() << ". Could not get or create channel: id:" << channelId.toString() << ", name:" << name)
#define LOG_G1015  LOG_WARN("G1015", LOG_TAG, "s:" << Core::socket() << ". Forbidden: id:" << ChatId(channel->id()).toString() << ", name:" << channel->name())

NodeChannels::NodeChannels(Core *core)
  : QObject(core)
  , NodeNoticeReader(Notice::ChannelType, core)
  , m_packet(0)
{
  connect(FeedEvents::i(), SIGNAL(notify(FeedEvent)), SLOT(notify(FeedEvent)));
}


bool NodeChannels::read(PacketReader *reader)
{
  if (SimpleID::typeOf(reader->sender()) != SimpleID::UserId)
    return false;

  m_user = Ch::channel(reader->sender(), SimpleID::UserId);
  if (!m_user)
    return false;

  ChannelNotice packet(m_type, reader);
  m_packet = &packet;

  const QString cmd = m_packet->command();
  int status  = Notice::NotImplemented;

  LOG_G1010

  if (cmd == CHANNELS_INFO_CMD)
    return info();

  else if (cmd == CHANNELS_JOIN_CMD)
    return join(m_packet->channelId, m_packet->text());

  else if (cmd == CHANNELS_PART_CMD)
    return part();

  else if (cmd == CHANNELS_QUIT_CMD)
    return quit();

  else if (cmd == CHANNELS_UPDATE_CMD)
    status = update();

  else if (cmd == CHANNELS_NAME_CMD)
    status = name();

  if (status == Notice::OK)
    return false;

  reply(status);
  return false;
}


void NodeChannels::acceptImpl(ChatChannel user, const AuthResult & /*result*/, QList<QByteArray> &packets)
{
  m_user = user;
  if (user->sockets().size() <= 1)
    m_user->channels().restore(DataBase::value(SimpleID::encode(m_user->id()) + LS("/channels")).toByteArray());

  packets.append(reply(Ch::server())->data(Core::stream()));
  packets.append(reply(m_user)->data(Core::stream()));

  if (m_user->channels().size())
    m_pending.append(user);

  if (m_pending.size() == 1)
    QTimer::singleShot(0, this, SLOT(join()));
}


void NodeChannels::addImpl(ChatChannel user)
{
  m_core->send(Sockets::all(user), ChannelNotice::info(user));
}


/*!
 * Обработка отключения пользователя.
 */
void NodeChannels::releaseImpl(ChatChannel user, quint64 socket)
{
  Q_UNUSED(socket);

  if (user->sockets().size())
    return;

  m_core->send(Sockets::all(user), ChannelNotice::request(user->id(), user->id(), CHANNELS_QUIT_CMD));

  const QList<QByteArray> channels = user->channels().all(SimpleID::ChannelId);

  foreach (const QByteArray &id, channels) {
    ChatChannel channel = Ch::channel(id);
    if (channel) {
      channel->removeChannel(user->id(), Ch::server()->feed(FEED_NAME_SERVER)->data().value(SERVER_FEED_OFFLINE_KEY, true).toBool());
      user->removeChannel(id);
      Ch::gc(channel);
    }
  }
}


void NodeChannels::join()
{
  while (!m_pending.isEmpty()) {
    m_user = m_pending.takeFirst();
    const QList<QByteArray> channels = m_user->channels().all(SimpleID::ChannelId);
    foreach (const QByteArray &id, channels) {
      join(id);
    }
  }
}


/*!
 * Обработка изменений фида FEED_NAME_CHANNEL.
 */
void NodeChannels::notify(const FeedEvent &event)
{
  if (event.status != Notice::OK || event.name != FEED_NAME_CHANNEL || !(event.method == FEED_METHOD_PUT || event.method == FEED_METHOD_POST) || !NodeChannelFeed::isReservedKey(event.path))
    return;

  ChatChannel channel = Ch::channel(event.channel, SimpleID::typeOf(event.channel));
  if (!channel)
    return;

  const QList<quint64> sockets = channel->type() == SimpleID::UserId ? Sockets::all(channel, true) : Sockets::channel(channel);
  m_core->send(sockets, ChannelNotice::info(channel, event.date));
}


/*!
 * Обработка запроса пользователем информации о канале.
 */
bool NodeChannels::info()
{
  if (m_packet->channels.isEmpty())
    return false;

  LOG_G1011

  QList<QByteArray> packets;
  foreach (const QByteArray &id, m_packet->channels) {
    ChatChannel channel = Ch::channel(id, SimpleID::typeOf(id));
    if (channel) {
      LOG_G1012

      if (channel->type() == SimpleID::UserId) {
        channel->addChannel(m_user->id());
        m_user->addChannel(channel->id());
      }

      packets += reply(channel, isForbidden(channel), CHANNELS_INFO_CMD)->data(Core::stream());
    }
  }

  if (packets.isEmpty())
    return false;

  m_core->send(m_user->sockets(), packets);
  return false;
}


/*!
 * Обработка запроса пользователя подключения к каналу.
 */
bool NodeChannels::join(const ChatId &channelId, const QString &name)
{
  LOG_G1013

  ChatChannel channel;

  /// Если идентификатор канала корректный, функция пытается получить его по этому идентификатору.
  const int type = channelId.type();
  if (type != ChatId::InvalidId && !(type == ChatId::UserId && !name.isEmpty() && ChatId(m_user->id()) == channelId))
    channel = Ch::channel(channelId.toByteArray(), type);

  /// Если канал не удалось получить по идентификатору, будет произведена попытка создать обычный канал по имени.
  if (!channel && (type == ChatId::InvalidId || type == ChatId::ChannelId))
    channel = Ch::channel(name, m_user);

  if (!channel && type == ChatId::UserId)
    channel = Ch::channel(Normalize::toId(type, name), type);

  if (!channel) {
    LOG_G1014
    return false;
  }

  if (channel->type() == ChatId::ChannelId && isForbidden(channel)) {
    LOG_G1015
    m_core->send(m_user->sockets(), reply(channel, true));
    return false;
  }

  const bool notify = !channel->channels().contains(m_user->id());
  channel->addChannel(m_user->id());
  m_user->addChannel(channel->id());

  if (notify && channel->type() == ChatId::ChannelId)
    dump();

  m_core->send(m_user->sockets(), reply(channel, false, CHANNELS_CHANNEL_CMD, name));

  /// В случае необходимости всем пользователям в канале будет разослано уведомление в входе нового пользователя.
  if (notify && channel->channels().all().size() > 1 && channel->type() == ChatId::ChannelId)
    m_core->send(Sockets::channel(channel), ChannelNotice::channel(m_user, channel->id(), CHANNELS_JOINED_CMD));

  return false;
}


/*!
 * Установка имени канала.
 *
 * \deprecated Этот метод изменения имени канала является устаревшим с версии 2.0.5.
 * Рекомендуется использовать \b put запрос к полю \b name фида \b channel.
 */
int NodeChannels::name()
{
  ChatChannel channel = Ch::channel(m_packet->channelId, SimpleID::typeOf(m_packet->channelId));
  if (!channel)
    return Notice::NotFound;

  FeedPtr feed = channel->feed(FEED_NAME_CHANNEL, false);
  if (!feed)
    return Notice::InternalError;

  return FeedsCore::put(channel.data(), CHANNEL_FEED_NAME_REQ, m_user.data(), m_packet->text(), Feed::Echo | Feed::Share | Feed::Broadcast).status;
}


/*!
 * Обработка отключения пользователя от канала.
 */
bool NodeChannels::part()
{
  ChatChannel channel = Ch::channel(m_packet->channelId, SimpleID::typeOf(m_packet->channelId));
  if (!channel)
    return false;

  m_user->removeChannel(channel->id());

  if (!channel->channels().all().contains(m_user->id()))
    return false;

  m_core->send(Sockets::channel(channel), ChannelNotice::request(m_user->id(), channel->id(), CHANNELS_PART_CMD));
  channel->removeChannel(m_user->id());

  if (channel->type() == SimpleID::ChannelId)
    dump();

  Ch::gc(channel);
  return false;
}


bool NodeChannels::quit()
{
  m_core->send(QList<quint64>() << m_core->packetsEvent()->socket(), QByteArray(), NewPacketsEvent::KillSocketOption);
  return false;
}


/*!
 * Обработка получения обновлённой информации о пользователе.
 *
 * \deprecated Этот метод изменения информации о пользователе является устаревшим с версии 2.0.5.
 */
int NodeChannels::update()
{
  if (m_user->id() != m_packet->sender())
    return Notice::Forbidden;

  FeedPtr feed = m_user->feed(FEED_NAME_CHANNEL, false);
  if (!feed)
    return Notice::InternalError;

  int updates       = 0;
  const int options = Feed::Echo | Feed::Share | Feed::Broadcast;

  if (m_user->name() != m_packet->text() && FeedsCore::put(m_user.data(), CHANNEL_FEED_NAME_REQ, m_user.data(), m_packet->text(), options).status == Notice::OK)
    updates++;

  if (m_user->gender().raw() != m_packet->gender && FeedsCore::put(m_user.data(), CHANNEL_FEED_GENDER_REQ, m_user.data(), m_packet->gender, options).status == Notice::OK)
    updates++;

  if (m_user->status().value() != m_packet->channelStatus && FeedsCore::put(m_user.data(), CHANNEL_FEED_STATUS_REQ, m_user.data(), m_packet->channelStatus, options).status == Notice::OK)
    updates++;

  if (!updates)
    return Notice::NotModified;

  return Notice::OK;
}


/*!
 * Возвращает \b true если пользователю запрещён доступ в канала \p channel.
 */
bool NodeChannels::isForbidden(ChatChannel channel) const
{
  FeedPtr feed = channel->feed(FEED_NAME_ACL, false);
  return (feed && !feed->can(m_user.data(), Acl::Read));
}


/*!
 * Отправка информации о канале.
 */
ChannelPacket NodeChannels::reply(ChatChannel channel, bool forbidden, const QString &command, const QString &xName) const
{
  ChannelPacket packet(new ChannelNotice(channel->id(), m_user->id(), command, DateTime::utc()));
  packet->setDirection(Notice::Server2Client);
  packet->setText(channel->name());
  packet->gender        = channel->gender().raw();
  packet->channelStatus = channel->status().value();

  if (channel->type() == SimpleID::ChannelId) {
    if (!forbidden) {
      packet->channels = channel->channels().all();
      if (Ch::server()->feed(FEED_NAME_SERVER)->data().value(SERVER_FEED_OFFLINE_KEY, true).toBool())
        packet->channels.append(channel->offline().all());
    }
    else
      packet->setStatus(Notice::Forbidden);
  }
  else if (m_user == channel)
    packet->channels = channel->channels().all(SimpleID::ChannelId);

  if (packet->status() == Notice::OK) {
    QVariantMap data = channel->feeds().f(m_user.data());
    if (!xName.isEmpty())
      data.insert(LS("x-name"), xName);

    packet->setData(data);
  }

  return packet;
}


void NodeChannels::dump() const
{
  const QList<QByteArray> channels = m_user->channels().all(SimpleID::ChannelId);
  QByteArray data;
  data.resize(channels.size() * SimpleID::DefaultSize);

  foreach (const QByteArray &id, channels)
    data.append(id);

  DataBase::setValue(SimpleID::encode(m_user->id()) + LS("/channels"), data);
}


void NodeChannels::reply(int status)
{
  m_core->send(m_user->sockets(), ChannelNotice::reply(*m_packet, status));
}
