/* $Id: CacheFeedStorage.cpp 3413 2013-01-17 13:19:45Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#include <QSqlQuery>
#include <QSqlDatabase>

#include "CacheDB.h"
#include "ChatNotify.h"
#include "feeds/CacheFeedStorage.h"
#include "JSON.h"
#include "net/packets/Notice.h"
#include "sglobal.h"

CacheFeedStorage::CacheFeedStorage(QObject *parent)
  : FeedStorage(parent)
{
}


/*!
 * Реализация сохранения фида.
 *
 * \param feed Фид.
 * \param date Дата.
 */
int CacheFeedStorage::saveImpl(FeedPtr feed, qint64 /*date*/)
{
  CacheDB::add(feed);
  return Notice::OK;
}


/*!
 * Реализация загрузки фида.
 */
void CacheFeedStorage::loadImpl(Channel *channel)
{
  qint64 key = CacheDB::key(channel->id());
  if (key <= 0)
    return;

  QSqlQuery query(QSqlDatabase::database(CacheDB::id()));
  query.prepare(LS("SELECT date, name, json FROM feeds WHERE channel = :channel;"));
  query.bindValue(LS(":channel"), key);
  query.exec();

  while (query.next()) {
    const QString name = query.value(1).toString();

    Feed *feed = FeedStorage::load(name, JSON::parse(query.value(2).toByteArray()).toMap());
    feed->head().setDate(query.value(0).toLongLong());
    channel->feeds().add(feed, false);

    ChatNotify::start(new FeedNotify(Notify::FeedData, channel->id(), name, QVariantMap(), Notice::Found));
  }
}


void CacheFeedStorage::removeImpl(FeedPtr feed)
{
  qint64 channel = CacheDB::key(feed->head().channel()->id());
  if (channel <= 0)
    return;

  QSqlQuery query(QSqlDatabase::database(CacheDB::id()));
  query.prepare(LS("SELECT id FROM feeds WHERE channel = :channel AND name = :name LIMIT 1;"));
  query.bindValue(LS(":channel"), channel);
  query.bindValue(LS(":name"), feed->head().name());
  query.exec();

  if (!query.first())
    return;

  qint64 key = query.value(0).toLongLong();
  if (key <= 0)
    return;

  query.prepare(LS("DELETE FROM feeds WHERE id = :id;"));
  query.bindValue(LS(":id"), key);
  query.exec();
}
