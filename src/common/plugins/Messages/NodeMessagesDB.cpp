/* $Id: NodeMessagesDB.cpp 3743 2013-07-10 13:42:02Z IMPOMEZIA $
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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThreadPool>
#include <QSqlError>
#include <QTimer>
#include <QTime>

#include "cores/Core.h"
#include "DataBase.h"
#include "net/packets/MessageNotice.h"
#include "NodeLog.h"
#include "NodeMessagesDB.h"
#include "sglobal.h"
#include "Storage.h"
#include "text/PlainTextFilter.h"

NodeMessagesDB *NodeMessagesDB::m_self = 0;
QString NodeMessagesDB::m_id;

#define LOG_M1010 LOG_FATAL("M1010", "Messages/DB", "Could not open DataBase file \"" << db.databaseName() << "\". " << NodeLog::toString(db.lastError()))
#define LOG_M1011 LOG_TRACE("M1011", "Messages/DB", "Adding new message: id:" << oid.toString() << ", status:" << m_packet.status() << ", sender:" << ChatId(m_packet.sender()).toString() << ", dest:" << ChatId(m_packet.dest()).toString() << ", date:" << m_packet.date() << ", mdate:" << m_packet.mdate << ", cmd:" << m_packet.command())
#define LOG_M1012 LOG_ERROR("M1012", "Messages/DB", "Failed add message: id:" << oid.toString() << ", status:" << m_packet.status() << ", sender:" << ChatId(m_packet.sender()).toString() << ", dest:" << ChatId(m_packet.dest()).toString() << " " << NodeLog::toString(query.lastError()))
#define LOG_M1013 LOG_TRACE("M1013", "Messages/DB", "Getting the latest messages: ch:" << channel.toString() << ", limit:" << limit << ", before:" << before)
#define LOG_M1014  LOG_WARN("M1014", "Messages/DB", "Id: " << channel.toString() << " is not channel id")
#define LOG_M1015 LOG_TRACE("M1015", "Messages/DB", "Getting latest messages: u1:" << user1.toString() << ", u2" << user2.toString() << ", limit:" << limit << ", before:" << before)
#define LOG_M1016  LOG_WARN("M1016", "Messages/DB", "Id: " << user1.toString() << " or " << user2.toString() << " is invalid")
#define LOG_M1017 LOG_TRACE("M1017", "Messages/DB", "Received " << out.size() << " IDs")
#define LOG_M1018 LOG_TRACE("M1018", "Messages/DB", "Get messages: count:" << ids.size() << ", userId:" << userId.toString())
#define LOG_M1019  LOG_WARN("M1019", "Messages/DB", "Failed get message: id:" << id.toString() << " " << NodeLog::toString(query.lastError()))
#define LOG_M1020 LOG_TRACE("M1020", "Messages/DB", "Permanently remove row: " << id)
#define LOG_M1021  LOG_WARN("M1021", "Messages/DB", "Failed remove row:" << id)
#define LOG_M1022 LOG_TRACE("M1022", "Messages/DB", "Updating existing message: id:" << m_record.oid.toString() << ", status:" << m_record.status << ", sender:" << m_record.sender.toString() << ", dest:" << m_record.dest.toString() << ", date:" << m_record.date << ", mdate:" << m_record.mdate << ", cmd:" << m_record.cmd)
#define LOG_M1023 LOG_ERROR("M1023", "Messages/DB", "Failed update message: id:" << m_record.oid.toString() << ", status:" << m_record.status << ", sender:" << m_record.sender.toString() << ", dest:" << m_record.dest.toString() << " " << NodeLog::toString(query.lastError()))

NodeMessagesDB::NodeMessagesDB(QObject *parent)
  : QObject(parent)
{
  m_self = this;
  m_id = LS("messages");
}


NodeMessagesDB::~NodeMessagesDB()
{
  m_self = 0;
  m_id.clear();
}


bool NodeMessagesDB::open()
{
  QSqlDatabase db = QSqlDatabase::addDatabase(LS("QSQLITE"), m_id);
  db.setDatabaseName(Storage::var() + LS("/messages.sqlite"));
  if (!db.open()) {
    LOG_M1010
    return false;
  }

  QSqlQuery query(db);
  query.exec(LS("PRAGMA synchronous = OFF"));

  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS messages ( "
    "  id     INTEGER PRIMARY KEY,"
    "  oid    BLOB NOT NULL UNIQUE,"
    "  sender INTEGER NOT NULL,"
    "  dest   INTEGER NOT NULL,"
    "  status INTEGER NOT NULL DEFAULT ( 200 ),"
    "  date   INTEGER NOT NULL DEFAULT ( 0 ),"
    "  mdate  INTEGER NOT NULL DEFAULT ( 0 ),"
    "  cmd    TEXT,"
    "  text   TEXT,"
    "  data   BLOB,"
    "  blob   BLOB"
    ");"
  ));

  query.exec(LS(
    "CREATE INDEX IF NOT EXISTS idx_messages ON messages ( "
    "  sender,"
    "  dest,"
    "  date,"
    "  mdate"
    ");"
  ));

  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS channels ( "
    "  id         INTEGER PRIMARY KEY,"
    "  channel    BLOB NOT NULL UNIQUE"
    ");"
  ));

  version();
  return true;
}


int NodeMessagesDB::status(int status)
{
  if (status == Notice::OK)
    return Notice::Found;

  if (status == Notice::ChannelOffline)
    return Notice::Unread;

  return status;
}


/*!
 * Получение идентификаторов последних сообщений для обычного канала.
 *
 * \param channel Идентификатор канала.
 * \param limit   Максимальное количество сообщений.
 * \param before  Дата, для загрузки сообщений старее этой даты.
 */
QList<ChatId> NodeMessagesDB::last(const ChatId &channel, const int limit, const qint64 before)
{
  LOG_M1013

  if (channel.type() != ChatId::ChannelId) {
    LOG_M1014
    return QList<ChatId>();
  }

  const qint64 dest = m_self->m_channels.get(channel);
  if (!dest)
    return QList<ChatId>();

  QSqlQuery query(QSqlDatabase::database(m_id));

  if (before) {
    query.prepare(LS("SELECT oid, mdate FROM messages WHERE dest = :dest AND date < :before ORDER BY id DESC LIMIT :limit;"));
    query.bindValue(LS(":before"), before);
  }
  else
    query.prepare(LS("SELECT oid, mdate FROM messages WHERE dest = :dest ORDER BY id DESC LIMIT :limit;"));

  query.bindValue(LS(":dest"),  dest);
  query.bindValue(LS(":limit"), limit);
  query.exec();

  return ids(query);
}


/*!
 * Получение идентификаторов последних приватных сообщений между двумя пользователями.
 *
 * \param user1  Идентификатор одного пользователя.
 * \param user2  Идентификатор другого пользователя.
 * \param limit  Максимальное количество сообщений.
 * \param before Дата, для загрузки сообщений старее этой даты.
 */
QList<ChatId> NodeMessagesDB::last(const ChatId &user1, const ChatId &user2, const int limit, const qint64 before)
{
  LOG_M1015

  const qint64 u1 = m_self->m_channels.get(user1);
  const qint64 u2 = m_self->m_channels.get(user2);
  if (!u1 || !u2) {
    LOG_M1016
    return QList<ChatId>();
  }

  QSqlQuery query(QSqlDatabase::database(m_id));
  if (before) {
    query.prepare(LS("SELECT oid, mdate FROM messages WHERE ((sender = :id1 AND dest = :id2) OR (sender = :id3 AND dest = :id4)) AND date < :before ORDER BY id DESC LIMIT :limit;"));
    query.bindValue(LS(":before"), before);
  }
  else
    query.prepare(LS("SELECT oid, mdate FROM messages WHERE (sender = :id1 AND dest = :id2) OR (sender = :id3 AND dest = :id4) ORDER BY id DESC LIMIT :limit;"));

  query.bindValue(LS(":id1"), u1);
  query.bindValue(LS(":id2"), u2);
  query.bindValue(LS(":id3"), u2);
  query.bindValue(LS(":id4"), u1);
  query.bindValue(LS(":limit"), limit);
  query.exec();

  return ids(query);
}


QList<ChatId> NodeMessagesDB::since(const ChatId &channel, const qint64 start, const qint64 end)
{
  if (channel.type() != ChatId::ChannelId)
    return QList<ChatId>();

  const qint64 dest = m_self->m_channels.get(channel);
  if (!dest)
    return QList<ChatId>();

  QSqlQuery query(QSqlDatabase::database(m_id));

  query.prepare(LS("SELECT oid, mdate FROM messages WHERE date > :start AND date < :end AND dest = :dest ORDER BY id DESC;"));
  query.bindValue(LS(":dest"),  dest);
  query.bindValue(LS(":start"), start);
  query.bindValue(LS(":end"),   end);
  query.exec();

  return ids(query);
}


QList<ChatId> NodeMessagesDB::since(const ChatId &user1, const ChatId &user2, const qint64 start, const qint64 end)
{
  const qint64 u1 = m_self->m_channels.get(user1);
  const qint64 u2 = m_self->m_channels.get(user2);
  if (!u1 || !u2)
    return QList<ChatId>();

  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("SELECT oid, mdate FROM messages WHERE date > :start AND date < :end AND ((sender = :id1 AND dest = :id2) OR (sender = :id3 AND dest = :id4)) ORDER BY id DESC;"));

  query.bindValue(LS(":id1"),   u1);
  query.bindValue(LS(":id2"),   u2);
  query.bindValue(LS(":id3"),   u2);
  query.bindValue(LS(":id4"),   u1);
  query.bindValue(LS(":start"), start);
  query.bindValue(LS(":end"),   end);
  query.exec();

  return ids(query);
}


/*!
 * Получения списка сообщений по их идентификаторам.
 */
QList<MessageRecordV2> NodeMessagesDB::get(const QList<ChatId> &ids, const ChatId &userId)
{
  if (ids.isEmpty())
    return QList<MessageRecordV2>();

  LOG_M1018

  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("SELECT id, sender, dest, status, date, mdate, cmd, text, data, blob FROM messages WHERE oid = :oid LIMIT 1;"));

  QList<MessageRecordV2> out;
# if QT_VERSION >= 0x040700
  out.reserve(ids.size());
# endif

  MessageRecordV2 record;

  for (int i = 0; i < ids.size(); ++i) {
    const ChatId &id = ids.at(i);

    query.bindValue(LS(":oid"), id.hasOid() ? ChatId::toBase32(id.oid().byteArray()) : id.toBase32());
    query.exec();

    if (!query.first()) {
      LOG_M1019
      continue;
    }

    record.id        = query.value(0).toLongLong();
    record.oid       = id;
    record.sender    = m_self->m_channels.get(query.value(1).toLongLong());
    record.dest      = m_self->m_channels.get(query.value(2).toLongLong());
    if (!userId.isNull() && (record.sender != userId && record.dest != userId))
      continue;

    record.status    = query.value(3).toLongLong();
    record.date      = query.value(4).toLongLong();
    record.mdate     = query.value(5).toLongLong();
    record.cmd       = query.value(6).toString();
    record.text      = query.value(7).toString();
    record.data      = query.value(8).toByteArray();
    record.blob      = query.value(9).toByteArray();

    out.append(record);
  }

  return out;
}


QList<MessageRecordV2> NodeMessagesDB::offline(const ChatId &userId)
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("SELECT id, oid, sender, dest, status, date, mdate, cmd, text, data, blob FROM messages WHERE dest = :dest AND status = 301 ORDER BY id DESC;"));
  query.bindValue(LS(":dest"), m_self->m_channels.get(userId));
  query.exec();

  return messages(query);
}


void NodeMessagesDB::add(const MessageNotice &packet, int status)
{
  AddMessageTask *task = new AddMessageTask(packet, status);
  m_self->m_tasks.append(task);
  if (m_self->m_tasks.size() == 1)
    QTimer::singleShot(0, m_self, SLOT(startTasks()));
}


void NodeMessagesDB::markAsRead(const QList<MessageRecordV2> &records)
{
  if (records.isEmpty())
    return;

  QSqlDatabase db = QSqlDatabase::database(m_id);
  QSqlQuery query(db);
  db.transaction();
  query.prepare(LS("UPDATE messages SET status = 302 WHERE id = :id;"));

  for (int i = 0; i < records.size(); ++i) {
    const MessageRecordV2& record = records.at(i);
    if (!record.id)
      continue;

    query.bindValue(LS(":id"), record.id);
    query.exec();
  }

  db.commit();
}


void NodeMessagesDB::remove(qint64 id)
{
  LOG_M1020

  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("DELETE FROM messages WHERE id = :id;"));
  query.bindValue(LS(":id"), id);
  query.exec();

  if (query.numRowsAffected() < 1)
    LOG_M1021
}


void NodeMessagesDB::update(const MessageRecordV2 &record)
{
  UpdateMessageTask *task = new UpdateMessageTask(record);
  m_self->m_tasks.append(task);
  if (m_self->m_tasks.size() == 1)
    QTimer::singleShot(0, m_self, SLOT(startTasks()));
}


void NodeMessagesDB::startTasks()
{
  if (m_tasks.isEmpty())
    return;

  QThreadPool *pool = DataBase::pool();
  while (!m_tasks.isEmpty())
    pool->start(m_tasks.takeFirst());
}


ChatId NodeMessagesDB::ChannelsCache::get(qint64 key)
{
  {
    QMutexLocker locker(&m_mutex);
    if (m_backward.contains(key))
      return m_backward.value(key);
  }

  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("SELECT channel FROM channels WHERE id = :id LIMIT 1;"));
  query.bindValue(LS(":id"), key);
  query.exec();

  ChatId id;
  if (query.first()) {
    id.init(query.value(0).toByteArray());
    add(id, key);
  }

  return id;
}


/*!
 * Поиск ключа в таблице \p channels для идентификатора канала \p id.
 *
 * Если ключ не найден в кэше, произойдёт запрос к базе данных для поиска ключа.
 * Если ключ не будет найден в базе, то идентификатор будет вставлен в базу.
 *
 * Эта функция потокобезопасна.
 */
qint64 NodeMessagesDB::ChannelsCache::get(const ChatId &id)
{
  if (id.type() != ChatId::ChannelId && id.type() != ChatId::UserId)
    return 0;

  m_mutex.lock();
  qint64 result = m_forward.value(id, 0);
  m_mutex.unlock();

  if (result)
    return result;

  QSqlQuery query(QSqlDatabase::database(m_id));
  query.prepare(LS("SELECT id FROM channels WHERE channel = :channel LIMIT 1;"));
  query.bindValue(LS(":channel"), id.toBase32());
  query.exec();

  if (query.first())
    return add(id, query.value(0).toLongLong());

  query.prepare(LS("INSERT INTO channels (channel) VALUES (:channel);"));
  query.bindValue(LS(":channel"), id.toBase32());
  query.exec();

  return add(id, query.lastInsertId().toLongLong());
}


qint64 NodeMessagesDB::ChannelsCache::add(const ChatId &id, qint64 value)
{
  if (!value)
    return 0;

  QMutexLocker locker(&m_mutex);
  m_forward.insert(id, value);
  m_backward.insert(value, id);
  return value;
}


/*!
 * Обновление схемы базы до версии 2.
 */
qint64 NodeMessagesDB::V2()
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.exec(LS("ALTER TABLE messages ADD data BLOB"));
  query.exec(LS("PRAGMA user_version = 2"));

  return 2;
}


qint64 NodeMessagesDB::V3()
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.exec(LS("BEGIN TRANSACTION;"));

  query.prepare(LS("SELECT id, messageId, senderId, destId FROM messages"));
  query.exec();

  QSqlQuery update(QSqlDatabase::database(m_id));
  update.prepare(LS("UPDATE messages SET messageId = :messageId, senderId = :senderId, destId = :destId WHERE id = :id;"));

  while (query.next()) {
    update.bindValue(LS(":id"),        query.value(0));
    update.bindValue(LS(":messageId"), SimpleID::encode(query.value(1).toByteArray()));
    update.bindValue(LS(":senderId"),  SimpleID::encode(query.value(2).toByteArray()));
    update.bindValue(LS(":destId"),    SimpleID::encode(query.value(3).toByteArray()));
    update.exec();
  }

  query.exec(LS("PRAGMA user_version = 3"));
  query.exec(LS("COMMIT;"));

  return 3;
}


qint64 NodeMessagesDB::V4()
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.exec(LS("BEGIN TRANSACTION;"));
  query.exec(LS("ALTER TABLE messages RENAME TO messages_tmp;"));
  query.exec(LS(
    "CREATE TABLE messages ( "
    "  id         INTEGER PRIMARY KEY,"
    "  messageId  BLOB NOT NULL UNIQUE,"
    "  senderId   BLOB NOT NULL,"
    "  destId     BLOB NOT NULL,"
    "  status     INTEGER DEFAULT ( 200 ),"
    "  date       INTEGER,"
    "  command    TEXT,"
    "  text       TEXT,"
    "  data       BLOB,"
    "  blob       BLOB"
    ");"
  ));

  query.exec(LS("INSERT INTO messages (messageId, senderId, destId, status, date, command, text, data) SELECT messageId, senderId, destId, status, date, command, text, data FROM messages_tmp;"));
  query.exec(LS("DROP TABLE messages_tmp;"));
  query.exec(LS("PRAGMA user_version = 4"));
  query.exec(LS("COMMIT;"));

  query.exec(LS("VACUUM;"));

  return 4;
}


/*!
 * Обновление схемы базы данных до версии 5.
 *
 * - Заполняется таблица \p channels и заполняется кэш каналов.
 * - Удаляется индекс \p idx_messages.
 * - Таблица \p messages переименовывается в \p messages_tmp.
 * - Таблица \p messages и индекс \p messages_tmp создаются заново.
 * - Сообщения из таблицы \p messages_tmp копируются в таблицу \p messages.
 * - Таблица \p messages_tmp удаляется.
 * - Выполняется VACUUM для уменьшения размера базы данных.
 */
qint64 NodeMessagesDB::V5()
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  ChannelsCache &cache = m_self->m_channels;

  {
    query.prepare(LS("SELECT senderId, destId FROM messages"));
    query.exec();

    ChatId id;
    QList<ChatId> channels;

    while (query.next()) {
      for (int i = 0; i < 2; ++i) {
        id.init(query.value(i).toByteArray());
        if (!channels.contains(id))
          channels.append(id);
      }
    }

    foreach (const ChatId &id, channels)
      cache.get(id);
  }

  query.exec(LS("DROP INDEX IF EXISTS idx_messages"));
  query.exec(LS("ALTER TABLE messages RENAME TO messages_tmp;"));
  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS messages ( "
    "  id     INTEGER PRIMARY KEY,"
    "  oid    BLOB NOT NULL UNIQUE,"
    "  sender INTEGER NOT NULL,"
    "  dest   INTEGER NOT NULL,"
    "  status INTEGER NOT NULL DEFAULT ( 200 ),"
    "  date   INTEGER NOT NULL DEFAULT ( 0 ),"
    "  mdate  INTEGER NOT NULL DEFAULT ( 0 ),"
    "  cmd    TEXT,"
    "  text   TEXT,"
    "  data   BLOB,"
    "  blob   BLOB"
    ");"
  ));

  query.exec(LS(
    "CREATE INDEX IF NOT EXISTS idx_messages ON messages ( "
    "  sender,"
    "  dest,"
    "  date,"
    "  mdate"
    ");"
  ));

  {
    query.exec(LS("SELECT messageId, senderId, destId, status, date, command, text, data FROM messages_tmp"));

    QSqlQuery insert(QSqlDatabase::database(m_id));
    insert.exec(LS("BEGIN TRANSACTION;"));
    insert.prepare(LS("INSERT INTO messages (oid,  sender,  dest,  status,  date,  cmd,  text,  data) "
                                   "VALUES (:oid, :sender, :dest, :status, :date, :cmd, :text, :data);"));

    qint64 sender = 0;
    qint64 dest = 0;
    ChatId id;

    while (query.next()) {
      sender = cache.get(id.init(query.value(1).toByteArray()));
      dest   = cache.get(id.init(query.value(2).toByteArray()));

      if (!sender || !dest)
        continue;

      insert.bindValue(LS(":oid"),    query.value(0));
      insert.bindValue(LS(":sender"), sender);
      insert.bindValue(LS(":dest"),   dest);
      insert.bindValue(LS(":status"), query.value(3));
      insert.bindValue(LS(":date"),   query.value(4));
      insert.bindValue(LS(":cmd"),    query.value(5));
      insert.bindValue(LS(":text"),   query.value(6));
      insert.bindValue(LS(":data"),   query.value(7));
      insert.exec();
    }

    insert.exec(LS("DROP TABLE messages_tmp;"));
    insert.exec(LS("PRAGMA user_version = 5"));
    insert.exec(LS("COMMIT;"));
    insert.exec(LS("VACUUM;"));
  }

  return 5;
}


QList<ChatId> NodeMessagesDB::ids(QSqlQuery &query)
{
  QList<ChatId> out;
  ChatId id;
  qint64 mdate = 0;

  while (query.next()) {
    id.init(query.value(0).toByteArray());
    mdate = query.value(1).toLongLong();
    if (mdate)
      id.setDate(mdate);

    out.prepend(id);
  }

  LOG_M1017
  return out;
}


QList<MessageRecordV2> NodeMessagesDB::messages(QSqlQuery &query)
{
  if (!query.isActive())
    return QList<MessageRecordV2>();

  QList<MessageRecordV2> out;

  while (query.next()) {
    MessageRecordV2 record;
    record.id        = query.value(0).toLongLong();
    record.oid       = ChatId(query.value(1).toByteArray());
    record.sender    = m_self->m_channels.get(query.value(2).toLongLong());
    record.dest      = m_self->m_channels.get(query.value(3).toLongLong());
    record.status    = query.value(4).toLongLong();
    record.date      = query.value(5).toLongLong();
    record.mdate     = query.value(6).toLongLong();
    record.cmd       = query.value(7).toString();
    record.text      = query.value(8).toString();
    record.data      = query.value(9).toByteArray();
    record.blob      = query.value(10).toByteArray();
    out.prepend(record);
  }

  return out;
}


/*!
 * Добавление в базу информации о версии, в будущем эта информация может быть использована для автоматического обновления схемы базы данных.
 */
void NodeMessagesDB::version()
{
  QSqlQuery query(QSqlDatabase::database(m_id));
  query.exec(LS("PRAGMA user_version"));
  if (!query.first())
    return;

  qint64 version = query.value(0).toLongLong();
  if (!version) {
    query.exec(LS("PRAGMA user_version = 5"));
    version = 5;
    return;
  }

  query.finish();

  if (version == 1) version = V2();
  if (version == 2) version = V3();
  if (version == 3) version = V4();
  if (version == 4) version = V5();
}


void AddMessageTask::run()
{
  const ChatId oid(m_packet.id());
  LOG_M1011

  QSqlQuery query(QSqlDatabase::database(NodeMessagesDB::m_id));
  query.prepare(LS("INSERT INTO messages (oid,  sender,  dest,  status,  date,  mdate,  cmd,  text,  data,  blob) "
                                "VALUES (:oid, :sender, :dest, :status, :date, :mdate, :cmd, :text, :data, :blob);"));

  query.bindValue(LS(":oid"),       m_packet.mdate ? ChatId::toBase32(oid.oid().byteArray()) : oid.toBase32());
  query.bindValue(LS(":sender"),    NodeMessagesDB::m_self->m_channels.get(ChatId(m_packet.sender())));
  query.bindValue(LS(":dest"),      NodeMessagesDB::m_self->m_channels.get(ChatId(m_packet.dest())));
  query.bindValue(LS(":status"),    NodeMessagesDB::status(m_status));
  query.bindValue(LS(":date"),      m_packet.date());
  query.bindValue(LS(":mdate"),     m_packet.mdate);
  query.bindValue(LS(":cmd"),       m_packet.command());
  query.bindValue(LS(":text"),      m_packet.text());
  query.bindValue(LS(":data"),      m_packet.raw());
  query.bindValue(LS(":blob"),      m_packet.blob());
  query.exec();

  if (query.numRowsAffected() < 1)
    LOG_M1012
}


void UpdateMessageTask::run()
{
  LOG_M1022

  QSqlQuery query(QSqlDatabase::database(NodeMessagesDB::m_id));
  query.prepare(LS("UPDATE messages SET status = :status, mdate = :mdate, text = :text, data = :data, blob = :blob WHERE id = :id;"));

  query.bindValue(LS(":status"), m_record.status);
  query.bindValue(LS(":mdate"),  m_record.mdate);
  query.bindValue(LS(":text"),   m_record.text);
  query.bindValue(LS(":data"),   m_record.data);
  query.bindValue(LS(":blob"),   m_record.blob);
  query.bindValue(LS(":id"),     m_record.id);
  query.exec();

  if (query.numRowsAffected() < 1)
    LOG_M1023
}
