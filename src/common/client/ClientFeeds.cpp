/* $Id: ClientFeeds.cpp 3590 2013-03-23 21:46:19Z IMPOMEZIA $
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

#include "acl/AclValue.h"
#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "client/ClientHooks.h"
#include "client/SimpleClient.h"
#include "feeds/FeedStrings.h"
#include "net/packets/FeedNotice.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "sglobal.h"

ClientFeeds::ClientFeeds(QObject *parent)
  : QObject(parent)
{
  m_hooks = new Hooks::Feeds(this);

  connect(ChatClient::io(), SIGNAL(notice(int)), SLOT(notice(int)));
}


bool ClientFeeds::del(const QByteArray &id, const QString &name, int options)
{
  QVariantMap json;
  json[FEED_KEY_OPTIONS] = options;
  return request(id, FEED_METHOD_DELETE, name, json);
}


bool ClientFeeds::get(const QByteArray &id, const QString &name, const QVariant &value)
{
  QVariantMap json;
  json[FEED_KEY_VALUE]   = value;
  return request(id, FEED_METHOD_GET, name, json);
}


bool ClientFeeds::post(const QByteArray &id, const QString &name, const QVariant &value, int options)
{
  QVariantMap json;
  json[FEED_KEY_VALUE]   = value;
  json[FEED_KEY_OPTIONS] = options;
  return request(id, FEED_METHOD_POST, name, json);
}


bool ClientFeeds::put(const QByteArray &id, const QString &name, const QVariant &value, int options)
{
  QVariantMap json;
  json[FEED_KEY_VALUE]   = value;
  json[FEED_KEY_OPTIONS] = options;
  return request(id, FEED_METHOD_PUT, name, json);
}


/*!
 * Запрос к фиду.
 *
 * Если фид не изменился, сервер вернут статус "303 Not Modified".
 *
 * \param channel Канал, к которому предназначен запрос.
 * \param command Команда отправленная фиду.
 * \param name    Имя фида с опциональным путём запроса.
 * \param json    Опциональные JSON данные.
 */
bool ClientFeeds::request(ClientChannel channel, const QString &command, const QString &name, const QVariantMap &json)
{
  if (!channel || command.isEmpty() || name.isEmpty())
    return false;

  FeedPacket packet = FeedNotice::request(ChatClient::id(), channel->id(), command, name, json);
  if (FeedNotice::split(name).second.isEmpty()) {
    FeedPtr feed = channel->feed(name, false);
    if (feed)
      packet->setDate(feed->head().date());
  }

  return ChatClient::io()->send(packet);
}


/*!
 * Запрос к фиду.
 *
 * \param id      Идентификатор канала, к которому предназначен запрос.
 * \param command Команда отправленная фиду.
 * \param name    Имя фида с опциональным путём запроса.
 * \param json    Опциональные JSON данные.
 */
bool ClientFeeds::request(const QByteArray &id, const QString &command, const QString &name, const QVariantMap &json)
{
  if (!Channel::isCompatibleId(id) || command.isEmpty() || name.isEmpty())
    return false;

  return ChatClient::io()->send(FeedNotice::request(ChatClient::id(), id, command, name, json));
}


/*!
 * Проверка прав доступа пользователя \p user в канале \p channel с учётом глобальных прав доступа.
 *
 * \return -1 если произошла ошибка или результирующие права доступа.
 */
int ClientFeeds::match(ClientChannel channel, ClientChannel user)
{
  if (!channel)
    return -1;

  FeedPtr feed = channel->feed(FEED_NAME_ACL, false);
  if (!feed)
    return -1;

  int acl = AclValue::match(feed.data(), user ? user.data() : 0);
  if (user && channel->type() != SimpleID::ServerId) {
    feed = ChatClient::server()->feed(FEED_NAME_ACL, false);
    if (feed) {
      const int serverAcl = AclValue::match(feed.data(), user.data());
      if (serverAcl & Acl::Edit) {
        acl |= Acl::Edit;
        acl |= Acl::Write;
      }

      if (serverAcl & Acl::SpecialEdit)
        acl |= Acl::SpecialEdit;
    }
  }

  return acl;
}


void ClientFeeds::notice(int type)
{
  if (type != Notice::FeedType)
    return;

  FeedNotice packet(type, ChatClient::io()->reader());
  if (!packet.isValid())
    return;

  m_packet = &packet;
  m_hooks->readFeed(packet);
}
