/* $Id: NodeConsoleFeed.cpp 3735 2013-07-07 23:38:33Z IMPOMEZIA $
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

#include "Account.h"
#include "Ch.h"
#include "Channel.h"
#include "DataBase.h"
#include "DateTime.h"
#include "feeds/ConsoleFeed.h"
#include "feeds/FeedsCore.h"
#include "feeds/NodeConsoleFeed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "Storage.h"

NodeConsoleFeed::NodeConsoleFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  m_header.acl().setMask(0444);
}


NodeConsoleFeed::NodeConsoleFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  m_header.acl().setMask(0444);
}


FeedReply NodeConsoleFeed::del(const QString &path, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (path.isEmpty() || !channel)
    return Notice::BadRequest;

  if (path == CONSOLE_FEED_ME_KEY) {
    if (!master(channel))
      return Notice::NotModified;

    return FeedReply(Notice::OK, del(Ch::channel(channel->id(), SimpleID::typeOf(channel->id()), false)));
  }
  else if (path.size() == 34) {
    const QByteArray id = SimpleID::decode(path);
    if (SimpleID::typeOf(id) != SimpleID::UserId)
      return Notice::BadRequest;

    ChatChannel user = Ch::channel(id, SimpleID::UserId);
    if (!user)
      return Notice::BadRequest;

    if (!user->account()->groups.contains(LS("master")))
      return Notice::NotModified;

    return FeedReply(Notice::OK, del(user));
  }

  return Notice::NotFound;
}


/*!
 * Обработка \b get запросов.
 */
FeedReply NodeConsoleFeed::get(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob) const
{
  Q_UNUSED(blob)

  if (path.isEmpty())
    return Notice::BadRequest;

  if (path == LS("head"))
    return FeedReply(Notice::OK, head().get(channel), head().date());

  if (path == CONSOLE_FEED_LOGIN_KEY)
    return login(json, channel);
  else if (path == CONSOLE_FEED_TRY_KEY)
    return tryAccess(channel);
  else if (path == CONSOLE_FEED_COOKIE_KEY)
    return cookie(json, channel);

  return Notice::NotFound;
}


/*!
 * \return \b true если пользователь входит в группу \b master.
 */
bool NodeConsoleFeed::master(Channel *user) const
{
  if (user && user->account()->groups.contains(LS("master")) && Storage::value(STORAGE_PASSWORD) != LS("2AZ6EKXDJCXLKZQPYIKAV3BVQUGE3KMXOA"))
    return true;

  return false;
}


FeedReply NodeConsoleFeed::cookie(const QVariantMap &json, Channel *user) const
{
  if (!master(user))
    return Notice::Forbidden;

  const QByteArray id = SimpleID::decode(json.value(FEED_KEY_VALUE).toString());
  if (SimpleID::typeOf(id) != SimpleID::UserId)
    return Notice::BadRequest;

  ChatChannel channel = Ch::channel(id, SimpleID::UserId);
  if (!channel || !channel->account())
    return Notice::NotFound;

  FeedReply reply(Notice::OK);
  reply.json[LS("user")]              = SimpleID::encode(id);
  reply.json[CONSOLE_FEED_COOKIE_KEY] = SimpleID::encode(channel->account()->cookie);
  return reply;
}


/*!
 * Проверка пароля сервера.
 */
FeedReply NodeConsoleFeed::login(const QVariantMap &json, Channel *user) const
{
  if (!user)
    return Notice::BadRequest;

  const QString password = json.value(STORAGE_PASSWORD).toString();
  if (password.isEmpty() || SimpleID::typeOf(SimpleID::decode(password)) != SimpleID::PasswordId)
    return Notice::BadRequest;

  if (Storage::value(STORAGE_PASSWORD) != password)
    return Notice::Forbidden;

  const qint64 date = DateTime::utc();
  user->account()->groups.add(LS("master"));
  user->account()->setDate(date);
  DataBase::add(Ch::channel(user->id(), SimpleID::typeOf(user->id()), false));

  FeedsCore::post(FEED_NAME_ACL + LS("/head/owner"), SimpleID::encode(user->id()), Feed::Broadcast);
  return FeedReply(Notice::OK, date);
}


/*!
 * Проверка привилегий пользователя, только пользователь в группе \b master имеет права доступа к фиду.
 */
FeedReply NodeConsoleFeed::tryAccess(Channel *user) const
{
  if (!master(user))
    return Notice::Forbidden;

  return FeedReply(Notice::OK, DateTime::utc());
}


/*!
 * Удаление пользователя из владельцев сервера.
 */
qint64 NodeConsoleFeed::del(ChatChannel user)
{
  const qint64 date = DateTime::utc();
  user->account()->groups.remove(LS("master"));
  user->account()->setDate(date);
  DataBase::add(user);

  FeedsCore::del(FEED_NAME_ACL + LS("/head/owner/") + SimpleID::encode(user->id()), Feed::Broadcast);
  return date;
}


Feed* NodeConsoleFeedCreator::create(const QString &name) const
{
  return new NodeConsoleFeed(name, DateTime::utc());
}


Feed* NodeConsoleFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeConsoleFeed(name, data);
}
