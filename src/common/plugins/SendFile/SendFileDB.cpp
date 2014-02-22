/* $Id: SendFileDB.cpp 2792 2012-06-24 15:42:54Z IMPOMEZIA $
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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QFileInfo>

#include "SendFileDB.h"
#include "net/SimpleID.h"
#include "sglobal.h"

SendFileDB::SendFileDB(QObject *parent)
  : QObject(parent)
{
}


bool SendFileDB::open(const QByteArray &id, const QString &dir)
{
  QString newId = SimpleID::encode(id) + LS("-sendfile");

  if (!m_id.isEmpty() && m_id == newId)
      return false;

  close();
  m_id = newId;

  QSqlDatabase db = QSqlDatabase::addDatabase(LS("QSQLITE"), m_id);
  db.setDatabaseName(dir + LS("/sendfile.sqlite"));
  if (!db.open())
    return false;

  create();
  return true;
}


void SendFileDB::close()
{
  QSqlDatabase::removeDatabase(m_id);
  m_id.clear();
}


void SendFileDB::restore(const QByteArray &id, SendFileTransaction transaction)
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("SELECT role, state, fileName, size FROM sendfile WHERE messageId = :messageId LIMIT 1;"));

  query.bindValue(LS(":messageId"), id);
  query.exec();

  if (!query.first())
    return;

  SendFile::Role role = static_cast<SendFile::Role>(query.value(0).toInt());
  SendFile::TransactionState state = static_cast<SendFile::TransactionState>(query.value(1).toInt());
  QString fileName = query.value(2).toString();

  if (role == SendFile::ReceiverRole && state == SendFile::FinishedState && !fileName.startsWith(LS("//"))) {
    QFileInfo info(fileName);
    if (!info.exists())
      return;

    if (info.size() != query.value(3).toLongLong())
      return;
  }

  transaction->setRole(role);
  transaction->setState(state);
  transaction->setFile(SendFile::File(fileName, query.value(3).toLongLong()));
}


void SendFileDB::save(const SendFileTransaction &transaction)
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("INSERT INTO sendfile (messageId, role, state, fileName, size) "
                     "VALUES (:messageId, :role, :state, :fileName, :size);"));

  query.bindValue(LS(":messageId"), transaction->id());
  query.bindValue(LS(":role"),      transaction->role());
  query.bindValue(LS(":state"),     transaction->state());
  query.bindValue(LS(":fileName"),  transaction->file().name);
  query.bindValue(LS(":size"),      transaction->file().size);
  query.exec();
}


void SendFileDB::create()
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.exec(LS("PRAGMA synchronous = OFF"));

  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS sendfile ( "
    "  id         INTEGER PRIMARY KEY,"
    "  messageId  BLOB,"
    "  role       INTEGER,"
    "  state      INTEGER,"
    "  fileName   TEXT,"
    "  size       INTEGER"
    ");"));

  version();
}


void SendFileDB::version()
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
}
