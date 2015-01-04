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
#include "feeds/FeedEvents.h"
#include "feeds/FeedsCore.h"
#include "feeds/FeedStorage.h"
#include "net/packets/FeedNotice.h"
#include "net/packets/Notice.h"
#include "Sockets.h"

FeedsCore *FeedsCore::m_self = 0;

FeedsCore::FeedsCore(QObject *parent)
  : QObject(parent)
{
  m_self = this;

  m_events = new FeedEvents(this);
  connect(m_events, SIGNAL(notify(FeedEvent)), SLOT(notify(FeedEvent)));

  sub(FEED_NAME_SERVER);
  sub(FEED_NAME_USERS);
}


FeedReply FeedsCore::post(const QString &name, const QVariant &value, int options)
{
  ServerChannel *server = Ch::server().data();
  return post(server, name, server, value, options);
}


FeedReply FeedsCore::post(const QString &name, ServerChannel *sender, const QVariant &value, int options)
{
  return post(Ch::server().data(), name, sender, value, options);
}


FeedReply FeedsCore::post(ServerChannel *channel, const QString &name, ServerChannel *sender, const QVariant &value, int options)
{
  QVariantMap json;
  json[FEED_KEY_VALUE]   = value;
  json[FEED_KEY_OPTIONS] = options;
  return request(channel, FEED_METHOD_POST, name, sender, json);
}


FeedReply FeedsCore::put(const QString &name, const QVariant &value, int options)
{
  ServerChannel *server = Ch::server().data();
  return put(server, name, server, value, options);
}


FeedReply FeedsCore::put(const QString &name, ServerChannel *sender, const QVariant &value, int options)
{
  return put(Ch::server().data(), name, sender, value, options);
}


FeedReply FeedsCore::put(ServerChannel *channel, const QString &name, ServerChannel *sender, const QVariant &value, int options)
{
  QVariantMap json;
  json[FEED_KEY_VALUE]   = value;
  json[FEED_KEY_OPTIONS] = options;
  return request(channel, FEED_METHOD_PUT, name, sender, json);
}


FeedReply FeedsCore::del(const QString &name, int options)
{
  ServerChannel *server = Ch::server().data();
  return del(server, name, server, options);
}


FeedReply FeedsCore::del(const QString &name, ServerChannel *sender, int options)
{
  return del(Ch::server().data(), name, sender, options);
}


FeedReply FeedsCore::del(ServerChannel *channel, const QString &name, ServerChannel *sender, int options)
{
  QVariantMap json;
  json[FEED_KEY_OPTIONS] = options;
  return request(channel, FEED_METHOD_DELETE, name, sender, json);
}


FeedReply FeedsCore::get(const QString &name, const QVariantMap &data)
{
  ServerChannel *server = Ch::server().data();
  return get(server, name, server, data);
}


FeedReply FeedsCore::get(const QString &name, ServerChannel *sender, const QVariantMap &data)
{
  return get(Ch::server().data(), name, sender, data);
}


FeedReply FeedsCore::get(ServerChannel *channel, const QString &name, ServerChannel *sender, const QVariantMap &data)
{
  return request(channel, FEED_METHOD_GET, name, sender, data);
}


/*!
 * Базовая функция совершения операции над фидом.
 *
 * \param channel Канал владелец фида.
 * \param method  Метод \sa Methods.
 * \param name    Имя фида с опциональным путём запроса.
 * \param sender  Канал создавший запрос.
 * \param json    Данные запроса.
 */
FeedReply FeedsCore::request(ServerChannel *channel, const QString &method, const QString &name, ServerChannel *sender, const QVariantMap &json)
{
  if (!channel || !sender)
    return Notice::InternalError;

  const QPair<QString, QString> split = FeedNotice::split(name);
  FeedPtr feed = channel->feed(split.first, false);
  if (!feed)
    return Notice::NotFound;

  FeedReply reply(Notice::InternalError);
  const int cmd    = methodToInt(method);
  FeedEvent *event = new FeedEvent(channel->id(), sender->id(), method);
  event->request   = json;
  event->name      = split.first;
  event->path      = split.second;
  event->date      = feed->head().date();

  if (!feed->can(sender, Acl::Read))
    return done(event, Notice::Forbidden);

  if (cmd != Get && !feed->can(sender, Acl::Write))
    return done(event, Notice::Forbidden);

  switch (cmd) {
    case Get:
      reply = feed->get(event->path, json, sender);
      break;

    case Post:
      reply = feed->post(event->path, json, sender);
      break;

    case Put:
      reply = feed->put(event->path, json, sender);
      break;

    case Delete:
      reply = feed->del(event->path, sender);
      break;
  }

  if (cmd != Get && reply.status == Notice::OK) {
    const int options = json.value(FEED_KEY_OPTIONS).toInt();
    event->diffTo     = event->date;
    event->date       = reply.date;

    if (reply.date)
      FeedStorage::save(feed, reply.date);

    if (options & Feed::Broadcast) {
      if (channel->type() == SimpleID::ServerId) {
        const QList<QByteArray> channels = m_self->m_subscription.value(event->name);
        QList<quint64> sockets;

        foreach (const QByteArray &id, channels) {
          ChatChannel user = Ch::channel(id, SimpleID::UserId, false);
          if (user)
            Sockets::merge(sockets, user->sockets());
        }

        event->broadcast = sockets;
      }
      else
        event->broadcast = Sockets::channel(channel);
    }
  }

  return done(event, reply);
}


void FeedsCore::sub(const QString &feedName)
{
  if (!m_self->m_subscription.contains(feedName))
    m_self->m_subscription[feedName] = QList<QByteArray>();
}


/*!
 * Обработка уведомлений фидов.
 */
void FeedsCore::notify(const FeedEvent &event)
{
  if (!event.broadcast.isEmpty())
    broadcast(event);

  if (SimpleID::typeOf(event.channel) != SimpleID::ServerId || !m_subscription.contains(event.name))
    return;

  if (event.socket && event.method == FEED_METHOD_GET && (event.status == Notice::OK || event.status == Notice::NotModified) && SimpleID::typeOf(event.sender) == SimpleID::UserId) {
    QList<QByteArray> &channels = m_subscription[event.name];
    if (!channels.contains(event.sender))
      channels.append(event.sender);
  }
  else if (!event.socket && event.method == FEED_METHOD_DELETE && event.name == FEED_NAME_USERS && event.path.size() == 34)
    m_subscription[event.name].removeAll(SimpleID::decode(event.path));
}


FeedReply FeedsCore::done(FeedEvent *event, const FeedReply &reply)
{
  event->reply  = reply.json;
  event->status = reply.status;

  FeedEvents::start(event);
  return reply;
}


/*!
 * Преобразование текстового представления метода в число.
 *
 * \sa Methods.
 */
int FeedsCore::methodToInt(const QString &method)
{
  if (method == FEED_METHOD_GET)
    return Get;
  else if (method == FEED_METHOD_POST)
    return Post;
  else if (method == FEED_METHOD_PUT)
    return Put;
  else if (method == FEED_METHOD_DELETE)
    return Delete;

  return Unknown;
}


/*!
 * Рассылка уведомления об изменении фида.
 */
void FeedsCore::broadcast(const FeedEvent &event)
{
  FeedPacket packet(new FeedNotice(event.channel, event.channel, FEED_METHOD_GET));
  packet->setDirection(Notice::Server2Client);
  packet->setText(FEED_WILDCARD_ASTERISK);

  QVariantMap json;
  json[event.name] = event.date;
  packet->setData(Feed::merge(FEED_KEY_F, json));

  Core::i()->send(event.broadcast, packet->data(Core::stream()));
}
