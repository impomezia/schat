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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "PreviewDB.h"
#include "PreviewItem.h"
#include "sglobal.h"

PreviewDB::PreviewDB(QObject *parent) :
  QObject(parent)
  , m_id(LS("preview-db"))
{
}


PreviewDB::~PreviewDB()
{
  QSqlDatabase::removeDatabase(m_id);
}


bool PreviewDB::open(const QString &path)
{
  QSqlDatabase db = QSqlDatabase::addDatabase(LS("QSQLITE"), m_id);
  db.setDatabaseName(path);
  if (!db.open())
    return false;

  create();
  return true;
}


ImageRecord PreviewDB::findById(const ChatId &id)
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("SELECT url, format, flags, width, height, size FROM images WHERE id = :id;"));
  query.bindValue(LS(":id"), id.toString());
  query.exec();

  ImageRecord record;

  if (!query.first())
    return record;

  record.id     = id;
  record.url    = query.value(0).toUrl();
  record.format = query.value(1).toString();
  record.flags  = query.value(2).toInt();
  record.width  = query.value(3).toInt();
  record.height = query.value(4).toInt();
  record.size   = query.value(5).toInt();

  return record;
}


void PreviewDB::clear()
{
  {
    QSqlQuery query(QSqlDatabase::database(m_id));
    query.exec(LS("DROP TABLE IF EXISTS images;"));
  }

  create();

  QSqlQuery query(QSqlDatabase::database(m_id));
  query.exec(LS("VACUUM;"));
}


void PreviewDB::save(const ChatId &id, const QUrl &url)
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("INSERT INTO images (id, url) VALUES (:id, :url);"));
  query.bindValue(LS(":id"),  id.toString());
  query.bindValue(LS(":url"), url.toString());
  query.exec();
}


void PreviewDB::save(PreviewItem *item)
{
  if (item->state() == PreviewItem::Error)
    return save(item->id(), item->url());

  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("INSERT INTO images (id, url, format, flags, width, height, size) VALUES (:id, :url, :format, :flags, :width, :height, :size);"));
  query.bindValue(LS(":id"),     item->id().toString());
  query.bindValue(LS(":url"),    item->url().toString());
  query.bindValue(LS(":format"), item->format());
  query.bindValue(LS(":flags"),  item->flags());
  query.bindValue(LS(":width"),  item->width());
  query.bindValue(LS(":height"), item->height());
  query.bindValue(LS(":size"),   item->size());
  query.exec();
}


void PreviewDB::create()
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.exec(LS("PRAGMA synchronous = OFF"));

  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS images ( "
    "  id         TEXT    PRIMARY KEY,"
    "  url        TEXT    NOT NULL,"
    "  format     TEXT,"
    "  flags      INTEGER DEFAULT ( 0 ),"
    "  width      INTEGER DEFAULT ( 0 ),"
    "  height     INTEGER DEFAULT ( 0 ),"
    "  size       INTEGER DEFAULT ( 0 ),"
    "  data       BLOB"
    ");"
  ));

  version();
}


void PreviewDB::version()
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.exec(LS("PRAGMA user_version"));
  if (!query.first())
    return;

  qint64 version = query.value(0).toLongLong();
  if (!version) {
    query.exec(LS("PRAGMA user_version = 1"));
    version = 1;
  }

  query.finish();
}
