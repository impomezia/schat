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

#include <QSqlQuery>

#include "DataBase.h"
#include "DateTime.h"
#include "feeds/NodeAclFeed.h"
#include "feeds/NodeChannelFeed.h"
#include "feeds/NodeFeedStorage.h"
#include "feeds/NodeHostsFeed.h"
#include "feeds/NodeProfileFeed.h"
#include "feeds/NodeServerFeed.h"
#include "feeds/NodeUserFeed.h"
#include "JSON.h"
#include "net/packets/Notice.h"
#include "ServerChannel.h"
#include "sglobal.h"

NodeFeedStorage::NodeFeedStorage(QObject *parent)
  : FeedStorage(parent)
{
  add(new NodeAclFeedCreator());
  add(new NodeHostsFeedCreator());
  add(new NodeUserFeedCreator());
  add(new NodeProfileFeedCreator());
  add(new NodeServerFeedCreator());
  add(new NodeChannelFeedCreator());

  start();
}


/*!
 * Реализация отката фида.
 *
 * Откат невозможен, если фид имеет только одну ревизию, или нужную ревизию не удалось
 * загрузить из базы данных.
 */
int NodeFeedStorage::revertImpl(FeedPtr feed, const QVariantMap &data)
{
  const FeedHeader& head = feed->head();
  qint64 rev = head.rev();
  if (!rev)
    return Notice::InternalError;

  qint64 revTo = data.value(LS("rev")).toLongLong();
  if (revTo == 0)
    revTo = rev - 1;

  QSqlQuery query;
  query.prepare(LS("SELECT json FROM feeds WHERE channel = :channel AND rev = :rev AND name = :name LIMIT 1;"));
  query.bindValue(LS(":channel"), head.channel()->key());
  query.bindValue(LS(":rev"),     revTo);
  query.bindValue(LS(":name"),    head.name());
  query.exec();

  if (!query.first())
    return Notice::InternalError;

  Feed *f = FeedStorage::load(head.name(), JSON::parse(query.value(0).toByteArray()).toMap());
  f->head().setRev(rev + 1);

  head.channel()->feeds().add(f);
  return Notice::OK;
}


/*!
 * Реализация сохранения фида.
 *
 * \param feed Фид.
 * \param date Дата.
 */
int NodeFeedStorage::saveImpl(FeedPtr feed, qint64 date)
{
  FeedHeader &head = feed->head();
  head.data().remove(LS("size"));
  head.data()[LS("date")] = date;
  head.setRev(rev(feed));

  QByteArray json = JSON::generate(feed->save());
  head.data()[LS("size")] = json.size();

  save(feed, json);
  if (head.key() <= 0)
    return Notice::InternalError;

  updateKey(feed);
  return Notice::OK;
}


/*!
 * Создание новой ревизии фида.
 *
 * \param feed Фид.
 */
void NodeFeedStorage::cloneImpl(FeedPtr feed)
{
  FeedHeader& head = feed->head();
  head.setRev(feed->head().rev() + 1);
  head.setKey(0);
  saveImpl(feed, DateTime::utc());
}


/*!
 * Реализация загрузки фида.
 */
void NodeFeedStorage::loadImpl(Channel *channel)
{
  QVariantMap feeds = channel->data().value(LS("feeds")).toMap();

  QMapIterator<QString, QVariant> i(feeds);
  while (i.hasNext()) {
    i.next();
    load(channel, i.key(), i.value().toLongLong());
  }
}


/*!
 * Удаление фида.
 */
void NodeFeedStorage::removeImpl(FeedPtr feed)
{
  FeedHeader &head = feed->head();
  QVariantMap feeds = head.channel()->data()[LS("feeds")].toMap();
  feeds[head.name()] = -head.data()[LS("rev")].toLongLong();
  head.channel()->setData(LS("feeds"), feeds);
  DataBase::saveData(head.channel());
}


qint64 NodeFeedStorage::rev(FeedPtr feed)
{
  const FeedHeader& head = feed->head();
  QVariantMap feeds = head.channel()->data().value(LS("feeds")).toMap();
  if (feeds.contains(head.name())) {
    qint64 rev = feeds.value(head.name()).toLongLong();
    if (rev <= 0)
      return qAbs(rev) + 1;
  }

  return head.rev();
}


/*!
 * Запись в базу тела фида.
 */
qint64 NodeFeedStorage::save(FeedPtr feed, const QByteArray &json)
{
  const FeedHeader &head = feed->head();
  if (head.key() > 0)
    return update(feed, json);

  QSqlQuery query;
  query.prepare(LS("INSERT INTO feeds (channel, rev, date, name, json) VALUES (:channel, :rev, :date, :name, :json);"));

  query.bindValue(LS(":channel"), head.channel()->key());
  query.bindValue(LS(":rev"),     head.rev());
  query.bindValue(LS(":date"),    head.date());
  query.bindValue(LS(":name"),    head.name());
  query.bindValue(LS(":json"),    json);
  query.exec();

  if (query.numRowsAffected() <= 0)
    return -1;

  qint64 key = query.lastInsertId().toLongLong();
  feed->head().setKey(key);
  return key;
}


qint64 NodeFeedStorage::update(FeedPtr feed, const QByteArray &json)
{
  const FeedHeader &head = feed->head();
  QSqlQuery query;
  query.prepare(LS("UPDATE feeds SET rev = :rev, date = :date, json = :json WHERE id = :id;"));
  query.bindValue(LS(":rev"),  head.rev());
  query.bindValue(LS(":date"), head.date());
  query.bindValue(LS(":json"), json);
  query.bindValue(LS(":id"),   head.key());
  query.exec();

  return head.key();
}


/*!
 * Загрузка фида из базы данных.
 *
 * \param channel Канал.
 * \param name    Имя фида.
 * \param id      Ключ в таблице фидов.
 */
void NodeFeedStorage::load(Channel *channel, const QString &name, qint64 id)
{
  if (id <= 0)
    return;

  QSqlQuery query;
  query.prepare(LS("SELECT json FROM feeds WHERE id = :id LIMIT 1;"));
  query.bindValue(LS(":id"), id);
  query.exec();

  if (!query.first())
    return;

  QByteArray data = query.value(0).toByteArray();
  QVariantMap json = JSON::parse(data).toMap();

  Feed *feed = FeedStorage::load(name, json);
  feed->head().data()[LS("size")] = data.size();
  feed->head().setKey(id);

  channel->feeds().add(feed, false);
}


void NodeFeedStorage::start()
{
  QSqlQuery query;

  query.exec(LS(
  "CREATE TABLE IF NOT EXISTS feeds ( "
  "  id         INTEGER PRIMARY KEY,"
  "  channel    INTEGER DEFAULT ( 0 ),"
  "  rev        INTEGER DEFAULT ( 0 ),"
  "  date       INTEGER DEFAULT ( 0 ),"
  "  name       TEXT    NOT NULL,"
  "  json       BLOB"
  ");"
  ));
}


/*!
 * Обновление информации о расположении фида в базе данных.
 */
void NodeFeedStorage::updateKey(FeedPtr feed)
{
  FeedHeader &head = feed->head();
  QVariantMap feeds = head.channel()->data().value(LS("feeds")).toMap();
  feeds[head.name()] = feed->head().key();
  head.channel()->setData(LS("feeds"), feeds);
  DataBase::saveData(head.channel());
}
