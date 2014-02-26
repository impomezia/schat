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

#include "DateTime.h"
#include "feeds/ListFeed.h"
#include "MasterDataCreator.h"
#include "net/NetRecord.h"
#include "sglobal.h"

bool MasterDataCreator::create(ChatChannel channel, const QString &path, NetRecord &record) const
{
  if (path == QString()) {
    QVariantMap map;
    map.insert(LS("name"),   channel->name());
    map.insert(LS("type"),   channel->type());

    int value = channel->status().value();
    if (value) map.insert(LS("status"), value);

    value = channel->gender().value();
    if (value) map.insert(LS("gender"), value);

    value = channel->gender().color();
    if (value) map.insert(LS("color"), value);

    record.date = channel->date();
    record.data = map;
    return true;
  }

  if (path == LS("sub")) {
    record.date = DateTime::utc();
    record.data = channel->subscribers().toData();
    return true;
  }

  if (channel->type() == ChatId::ServerId || channel->type() == ChatId::ChannelId) {
    if (path == LS("stats"))
      return stats(channel, record);

    if (path == LS("users"))
      return users(channel, record);
  }

  if (channel->type() != ChatId::ServerId)
    return false;

  if (path == LS("index")) {
    FeedPtr feed = channel->feed(FEED_NAME_LIST);
    record.date  = feed->head().date();
    record.data  = feed->data().value(LIST_FEED_CHANNELS_KEY);
    return true;
  }

  if (path == LS("server"))
    return server(channel, record);

  return false;
}


QStringList MasterDataCreator::paths() const
{
  QStringList out;
  out.append(QString());
  out.append(LS("index"));
  out.append(LS("sub"));
  out.append(LS("server"));
  out.append(LS("stats"));
  out.append(LS("users"));
  return out;
}


bool MasterDataCreator::server(ChatChannel channel, NetRecord &record) const
{
  FeedPtr feed = channel->feed(FEED_NAME_SERVER);
  record.date  = feed->head().date();
  const QVariantMap data = feed->data();

  QVariantMap out;
  out.insert(LS("id"),   data.value(LS("id")));
  out.insert(LS("name"), data.value(LS("name")));
  out.insert(LS("ver"),  data.value(LS("version")));
  out.insert(LS("os"),   QVariantList() << data.value(LS("os")));

  record.data = out;
  return true;
}


bool MasterDataCreator::stats(ChatChannel channel, NetRecord &record) const
{
  FeedPtr feed = channel->feed(FEED_NAME_STATS);
  record.date  = feed->head().date();
  record.data  = feed->data();
  return true;
}


bool MasterDataCreator::users(ChatChannel channel, NetRecord &record) const
{
  FeedPtr feed = channel->feed(FEED_NAME_USERS);
  record.date  = feed->head().date();
  const QList<QByteArray> users = channel->channels().all(ChatId::UserId);

  QStringList out;
  ChatId id;

  foreach (const QByteArray &userId, users)
    out.append(id.init(userId).toString());

  record.data = out;
  return true;
}
