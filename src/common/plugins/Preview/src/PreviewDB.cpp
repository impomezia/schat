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
#include "sglobal.h"

PreviewDB::PreviewDB(QObject *parent) :
  QObject(parent)
  , m_id(LS("preview-db"))
{
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


void PreviewDB::create()
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.exec(LS("PRAGMA synchronous = OFF"));

  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS images ( "
    "  id         TEXT    PRIMARY KEY,"
    "  url        TEXT    NOT NULL,"
    "  format     TEXT    NOT NULL,"
    "  width      INTEGER DEFAULT ( 0 ),"
    "  height     INTEGER DEFAULT ( 0 ),"
    "  size       INTEGER DEFAULT ( 0 ),"
    "  name       TEXT,"
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
