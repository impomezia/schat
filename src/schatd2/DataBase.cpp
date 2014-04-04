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

#include <QCoreApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QThreadPool>
#include <QTimer>
#include <QVariant>

#include "Ch.h"
#include "DataBase.h"
#include "DateTime.h"
#include "feeds/FeedStorage.h"
#include "feeds/FeedStrings.h"
#include "JSON.h"
#include "net/SimpleID.h"
#include "NodeLog.h"
#include "Normalize.h"
#include "Path.h"
#include "sglobal.h"
#include "Storage.h"
#include "tools/Ver.h"

DataBase *DataBase::m_self = 0;

#define LOG_N6010 LOG_FATAL("N6010", "Core/DataBase", "Unable to open database file: \"" << db.databaseName() << "\". " << NodeLog::toString(db.lastError()))
#define LOG_N6011 LOG_ERROR("N6011", "Core/DataBase", "Could not add channel: " << NodeLog::toString(query.lastError()))

AddHostTask::AddHostTask(Host *host)
  : QRunnable()
  , m_host(*host)
{
}


/*!
 * Запуск задачи.
 */
void AddHostTask::run()
{
  QSqlQuery query;
  query.prepare(LS("SELECT id FROM hosts WHERE hostId = :hostId LIMIT 1;"));
  query.bindValue(LS(":hostId"), SimpleID::encode(m_host.hostId));
  query.exec();

  qint64 key = -1;
  if (query.first())
    key = query.value(0).toLongLong();


  if (key == -1) {
    query.prepare(LS("INSERT INTO hosts (channel,  hostId,  name,  address,  version,  os,  osName,  tz,  date,  geo,  data)"
                               " VALUES (:channel, :hostId, :name, :address, :version, :os, :osName, :tz, :date, :geo, :data)"));

    query.bindValue(LS(":channel"), m_host.channel);
    query.bindValue(LS(":hostId"),  SimpleID::encode(m_host.hostId));
  }
  else {
    query.prepare(LS("UPDATE hosts SET name = :name, address = :address, version = :version, os = :os, osName = :osName, tz = :tz, date = :date, geo = :geo, data = :data WHERE id = :id;"));
    query.bindValue(LS(":id"),      key);
  }

  query.bindValue(LS(":name"),    m_host.name);
  query.bindValue(LS(":address"), m_host.address);
  query.bindValue(LS(":version"), Ver(m_host.version).toString());
  query.bindValue(LS(":os"),      m_host.os);
  query.bindValue(LS(":osName"),  m_host.osName);
  query.bindValue(LS(":tz"),      m_host.tz);
  query.bindValue(LS(":date"),    m_host.date);
  query.bindValue(LS(":geo"),     JSON::generate(m_host.geo));
  query.bindValue(LS(":data"),    JSON::generate(m_host.data));

  query.exec();
}


AddProfileTask::AddProfileTask(User *user)
  : QRunnable()
  , m_user(*user)
{
}


/*!
 * Запуск задачи.
 */
void AddProfileTask::run()
{
  QSqlQuery query;
  query.prepare(LS("SELECT id FROM profiles WHERE channel = :channel LIMIT 1;"));
  query.bindValue(LS(":channel"), m_user.channel);
  query.exec();

  qint64 key = -1;
  if (query.first())
    key = query.value(0).toLongLong();

  if (key == -1) {
    query.prepare(LS("INSERT INTO profiles (channel,  date,  name,  email,  city,  country,  link,  site,  birthday,  extra)"
                                  " VALUES (:channel, :date, :name, :email, :city, :country, :link, :site, :birthday, :extra)"));

    query.bindValue(LS(":channel"),  m_user.channel);
  }
  else {
    query.prepare(LS("UPDATE profiles SET date = :date, name = :name, email = :email, city = :city, country = :country,"
        "link = :link, site = :site, birthday = :birthday, extra = :extra WHERE id = :id;"));

    query.bindValue(LS(":id"), key);
  }

  query.bindValue(LS(":date"),     m_user.date);
  query.bindValue(LS(":name"),     m_user.name);
  query.bindValue(LS(":email"),    m_user.email);
  query.bindValue(LS(":city"),     m_user.city);
  query.bindValue(LS(":country"),  m_user.country);
  query.bindValue(LS(":link"),     m_user.link);
  query.bindValue(LS(":site"),     m_user.site);
  query.bindValue(LS(":birthday"), m_user.birthday);
  query.bindValue(LS(":extra"),    JSON::generate(m_user.extra));

  query.exec();
}


AddValueTask::AddValueTask(const QString &key, const QVariant &value)
  : QRunnable()
  , m_key(key)
  , m_value(value)
{
}


void AddValueTask::run()
{
  QSqlQuery query;
  query.prepare(LS("SELECT id FROM storage WHERE k = :k LIMIT 1;"));
  query.bindValue(LS(":k"), m_key);
  query.exec();

  qint64 key = -1;
  if (query.first())
    key = query.value(0).toLongLong();

  int format = AutoFormat;
  int type = m_value.type();
  if (type == QVariant::StringList || type == QVariant::List || type == QVariant::Map || type == QVariant::Hash)
    format = JSonFormat;

  if (key == -1) {
    query.prepare(LS("INSERT INTO storage (k, f, v) VALUES (:k, :f, :v)"));
    query.bindValue(LS(":k"), m_key);
  }
  else {
    query.prepare(LS("UPDATE storage SET f = :f, v = :v WHERE id = :id;"));
    query.bindValue(LS(":id"), key);
  }

  query.bindValue(LS(":f"), format);
  query.bindValue(LS(":v"), format == JSonFormat ? JSON::generate(m_value) : m_value);

  query.exec();
}


/*!
 * База данных сервера.
 */
DataBase::DataBase(QObject *parent)
  : QObject(parent)
{
  m_self = this;
  m_pool = new QThreadPool(this);
  m_pool->setMaxThreadCount(1);
}


DataBase::~DataBase()
{
  m_self = 0;

  QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
}


int DataBase::start()
{
  QSqlDatabase db = QSqlDatabase::addDatabase(LS("QSQLITE"));
  QDir().mkpath(Storage::var());

  db.setDatabaseName(Storage::var() + LC('/') + Path::app() + LS(".sqlite"));
  if (!db.open()) {
    LOG_N6010
    QCoreApplication::exit(1);
    return 1;
  }

  QSqlQuery query;
  query.exec(LS("PRAGMA synchronous = OFF"));
  QStringList tables = db.tables();

  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS accounts ( "
    "  id         INTEGER PRIMARY KEY,"
    "  channel    INTEGER UNIQUE,"
    "  date       INTEGER DEFAULT ( 0 ),"
    "  cookie     BLOB    NOT NULL UNIQUE,"
    "  provider   TEXT,"
    "  flags      INTEGER DEFAULT ( 0 ),"
    "  groups     TEXT,"
    "  data       BLOB"
    ");"
  ));

  if (!tables.contains(LS("groups"))) {
    query.exec(LS(
    "CREATE TABLE groups ( "
    "  id          INTEGER PRIMARY KEY,"
    "  name        TEXT    NOT NULL UNIQUE ,"
    "  permissions TEXT"
    ");"));

    addGroup(LS("master"));
    addGroup(LS("registered"));
    addGroup(LS("anonymous"));
  }

  if (!tables.contains(LS("channels"))) {
    query.exec(LS(
    "CREATE TABLE channels ( "
    "  id         INTEGER PRIMARY KEY,"
    "  channel    BLOB    NOT NULL UNIQUE,"
    "  normalized BLOB    NOT NULL UNIQUE,"
    "  type       INTEGER DEFAULT ( 73 ),"
    "  gender     INTEGER DEFAULT ( 0 ),"
    "  status     INTEGER DEFAULT ( 0 ),"
    "  account    INTEGER DEFAULT ( 0 ),"
    "  name       TEXT    NOT NULL,"
    "  data       BLOB,"
    "  date       INTEGER DEFAULT ( 0 )"
    ");"));
  }

  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS hosts ( "
    "  id         INTEGER PRIMARY KEY,"
    "  channel    INTEGER,"
    "  hostId     BLOB NOT NULL UNIQUE,"
    "  name       TEXT,"
    "  address    TEXT,"
    "  version    TEXT,"
    "  os         INTEGER,"
    "  osName     TEXT,"
    "  tz         INTEGER,"
    "  date       INTEGER,"
    "  geo        BLOB,"
    "  data       BLOB"
    ");"
  ));

  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS profiles ( "
    "  id         INTEGER PRIMARY KEY,"
    "  channel    INTEGER UNIQUE,"
    "  date       INTEGER DEFAULT ( 0 ),"
    "  name       TEXT,"
    "  email      TEXT,"
    "  city       TEXT,"
    "  country    TEXT,"
    "  link       TEXT,"
    "  site       TEXT,"
    "  birthday   TEXT,"
    "  extra      BLOB"
    ");"
  ));

  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS storage ( "
    "  id         INTEGER PRIMARY KEY,"
    "  k          TEXT UNIQUE,"
    "  f          INTEGER DEFAULT ( 0 ),"
    "  v          BLOB"
    ");"
  ));

  version();
  return 0;
}


qint64 DataBase::addGroup(const QString &name, const QString &permissions)
{
  QSqlQuery query;
  query.prepare(LS("INSERT INTO groups (name, permissions) VALUES (:name, :permissions);"));
  query.bindValue(LS(":name"), name);
  query.bindValue(LS(":permissions"), permissions);
  query.exec();

  if (query.numRowsAffected() <= 0)
    return -1;

  return query.lastInsertId().toLongLong();
}


/*!
 * Получение канала на основе идентификатора канала или нормализированного имени и типа канала.
 */
ChatChannel DataBase::channel(const QByteArray &id, int type)
{
  qint64 key = channelKey(id, type);
  if (key == -1)
    return ChatChannel();

  return channel(key);
}


/*!
 * Добавление канала.
 * Если канал уже добавлен, он будет обновлён.
 *
 * \param channel Указатель на канал.
 * \return Ключ в таблице \b channels или -1 в случае ошибки.
 *
 * \sa update().
 */
qint64 DataBase::add(ChatChannel channel)
{
  qint64 key = channel->key();
  if (key <= 0)
    key = channelKey(channel->id(), channel->type());

  if (key > 0) {
    if (channel->key() != key)
      channel->setKey(key);

    update(channel);
    return key;
  }

  QSqlQuery query;
  query.prepare(LS("INSERT INTO channels (channel, normalized, type, gender, name, data, date) "
                     "VALUES (:channel, :normalized, :type, :gender, :name, :data, :date);"));

  query.bindValue(LS(":channel"),    SimpleID::encode(channel->id()));
  query.bindValue(LS(":normalized"), SimpleID::encode(channel->normalized()));
  query.bindValue(LS(":type"),       channel->type());
  query.bindValue(LS(":gender"),     channel->gender().raw());
  query.bindValue(LS(":name"),       channel->name());
  query.bindValue(LS(":data"),       JSON::generate(channel->data()));
  query.bindValue(LS(":date"),       channel->date());
  query.exec();

  if (query.numRowsAffected() <= 0) {
    LOG_N6011
    return -1;
  }

  key = query.lastInsertId().toLongLong();
  channel->setKey(key);
  return key;
}


qint64 DataBase::isCollision(const QByteArray &id, const QByteArray &normalized)
{
  qint64 key = isCollision(id, normalized, SimpleID::UserId);
  if (!key)
    return 0;

  // Перезапись ника возможна только при регистрации.
  if (channelKey(id, SimpleID::UserId) != -1)
    return key;

  QSqlQuery query;
  query.prepare(LS("SELECT provider FROM accounts WHERE channel = :channel LIMIT 1;"));
  query.bindValue(LS(":channel"), key);
  query.exec();

  if (!query.first())
    return key;

  // Если имя провайдера не пустое, перезапись невозможна.
  if (!query.value(0).toString().isEmpty())
    return key;

  query.prepare(LS("SELECT name FROM channels WHERE id = :id LIMIT 1;"));
  query.bindValue(LS(":id"), key);
  query.exec();

  if (!query.first())
    return key;

  QString nick = query.value(0).toString().left(Channel::MaxNameLength - 3) + QString::number(qrand() % 899 + 100);
  const QByteArray n = Normalize::toId(SimpleID::UserId, nick);
  // Пользователь с новый ником уже существует.
  if (channelKey(n, SimpleID::UserId) != -1)
    return key;

  query.prepare(LS("UPDATE channels SET normalized = :normalized, name = :name WHERE id = :id;"));
  query.bindValue(LS(":normalized"), SimpleID::encode(n));
  query.bindValue(LS(":name"),       nick);
  query.bindValue(LS(":id"),         key);
  query.exec();

  if (!query.numRowsAffected())
    return key;

  return 0;
}


/*!
 * Прямое обращение к базе данных для проверки на коллизию имени канала.
 *
 * \param id         Идентификатор канала.
 * \param normalized Идентификатор нормализированного имени канала.
 * \param type       Тип канала.
 *
 * \return 0 если коллизия не обнаружена или ключ существующего канала в таблице \b channels.
 */
qint64 DataBase::isCollision(const QByteArray &id, const QByteArray &normalized, int type)
{
  QSqlQuery query;
  query.prepare(LS("SELECT id, channel FROM channels WHERE normalized = :normalized AND type = :type LIMIT 1;"));
  query.bindValue(LS(":normalized"), SimpleID::encode(normalized));
  query.bindValue(LS(":type"), type);
  query.exec();

  if (!query.first() || SimpleID::decode(query.value(1).toByteArray()) == id)
    return 0;

  return query.value(0).toLongLong();
}


/*!
 * Сохранение данных канала.
 */
void DataBase::saveData(Channel *channel)
{
  QSqlQuery query;
  query.prepare(LS("UPDATE channels SET data = :data WHERE id = :id;"));
  query.bindValue(LS(":data"),       JSON::generate(channel->data()));
  query.bindValue(LS(":id"),         channel->key());
  query.exec();
}


void DataBase::add(Account *account)
{
  if (!account || !account->channel)
    return;

  if (account->cookie.isEmpty()) {
    account->setDate(DateTime::utc());
    account->cookie = Ch::cookie();
  }

  if (account->saved)
    return;

  QSqlQuery query;
  query.prepare(LS("INSERT INTO accounts (channel, date, cookie, provider, flags, groups) "
                     "VALUES (:channel, :date, :cookie, :provider, :flags, :groups);"));

  query.bindValue(LS(":channel"),    account->channel);
  query.bindValue(LS(":date"),       account->date);
  query.bindValue(LS(":cookie"),     SimpleID::encode(account->cookie));
  query.bindValue(LS(":provider"),   account->provider);
  query.bindValue(LS(":flags"),      account->flags);
  query.bindValue(LS(":groups"),     account->groups.toString());
  query.exec();
}


/*!
 * Получения списка хостов связанных с каналом.
 */
QMap<QByteArray, HostInfo> DataBase::hosts(qint64 channel)
{
  QMap<QByteArray, HostInfo> out;

  QSqlQuery query;
  query.prepare(LS("SELECT hostId, name, address, version, os, osName, tz, date, geo, data FROM hosts WHERE channel = :channel;"));
  query.bindValue(LS(":channel"), channel);
  query.exec();

  while (query.next()) {
    HostInfo host(new Host());
    host->channel = channel;
    host->hostId  = SimpleID::decode(query.value(0).toByteArray());
    host->name    = query.value(1).toString();
    host->address = query.value(2).toString();
    host->version = Ver(query.value(3).toString()).toUInt();
    host->os      = query.value(4).toInt();
    host->osName  = query.value(5).toString();
    host->tz      = query.value(6).toInt();
    host->date    = query.value(7).toLongLong();
    host->geo     = JSON::parse(query.value(8).toByteArray()).toMap();
    host->data    = JSON::parse(query.value(9).toByteArray()).toMap();

    out[host->hostId] = host;
  }

  return out;
}


/*!
 * Добавление или обновление информации о хосте.
 */
void DataBase::add(HostInfo host)
{
  AddHostTask *task = new AddHostTask(host.data());
  m_self->m_tasks.append(task);
  if (m_self->m_tasks.size() == 1)
    QTimer::singleShot(0, m_self, SLOT(startTasks()));
}


void DataBase::removeHost(const QByteArray &hostId)
{
  QSqlQuery query;
  query.prepare(LS("DELETE FROM hosts WHERE hostId = :hostId;"));
  query.bindValue(LS(":hostId"), SimpleID::encode(hostId));
  query.exec();
}


void DataBase::add(User *user)
{
  if (!user->channel || user->saved)
    return;

  user->saved = true;

  AddProfileTask *task = new AddProfileTask(user);
  m_self->m_tasks.append(task);
  if (m_self->m_tasks.size() == 1)
    QTimer::singleShot(0, m_self, SLOT(startTasks()));
}


/*!
 * Получение профиля пользователя.
 */
User DataBase::user(qint64 channel)
{
  QSqlQuery query;
  query.prepare(LS("SELECT date, name, email, city, country, link, site, birthday, extra FROM profiles WHERE channel = :channel;"));
  query.bindValue(LS(":channel"), channel);
  query.exec();

  User out;
  if (!query.first())
    return out;

  out.channel  = channel;
  out.date     = query.value(0).toLongLong();
  out.name     = query.value(1).toString();
  out.email    = query.value(2).toString();
  out.city     = query.value(3).toString();
  out.country  = query.value(4).toString();
  out.link     = query.value(5).toString();
  out.site     = query.value(6).toString();
  out.birthday = query.value(7).toString();
  out.extra    = JSON::parse(query.value(8).toByteArray()).toMap();

  return out;
}


bool DataBase::contains(const QString &key)
{
  QSqlQuery query;
  query.prepare(LS("SELECT f FROM storage WHERE k = :k LIMIT 1;"));
  query.bindValue(LS(":k"), key);
  query.exec();

  return query.first();
}


QVariant DataBase::value(const QString &key, const QVariant &defaultValue)
{
  QSqlQuery query;
  query.prepare(LS("SELECT f, v FROM storage WHERE k = :k LIMIT 1;"));
  query.bindValue(LS(":k"), key);
  query.exec();

  if (!query.first())
    return defaultValue;

  int format = query.value(0).toInt();
  if (format == AddValueTask::JSonFormat)
    return JSON::parse(query.value(1).toByteArray());

  return query.value(1);
}


void DataBase::setValue(const QString &key, const QVariant &value)
{
  AddValueTask *task = new AddValueTask(key, value);
  m_self->m_tasks.append(task);
  if (m_self->m_tasks.size() == 1)
    QTimer::singleShot(0, m_self, SLOT(startTasks()));
}


void DataBase::startTasks()
{
  while (!m_tasks.isEmpty())
    m_pool->start(m_tasks.takeFirst());
}


/*!
 * Получение аккаунта пользователя.
 *
 * \param key Ключ канала.
 */
Account DataBase::account(qint64 key)
{
  QSqlQuery query;
  query.prepare(LS("SELECT channel, date, cookie, provider, flags, groups FROM accounts WHERE channel = :channel LIMIT 1;"));
  query.bindValue(LS(":channel"), key);
  query.exec();

  if (!query.first())
    return Account();

  Account account;
  account.channel  = query.value(0).toLongLong();
  account.date     = query.value(1).toLongLong();
  account.cookie   = SimpleID::decode(query.value(2).toByteArray());
  account.provider = query.value(3).toString();
  account.flags    = query.value(4).toLongLong();
  account.groups.set(query.value(5).toString());

  return account;
}


/*!
 * Получение канала на основе первичного ключа в таблице \b channels.
 */
ChatChannel DataBase::channel(qint64 id)
{
  QSqlQuery query;
  query.prepare(LS("SELECT channel, gender, status, name, data, date FROM channels WHERE id = :id LIMIT 1;"));
  query.bindValue(LS(":id"), id);
  query.exec();

  if (!query.first())
    return ChatChannel();

  ChatChannel channel(new ServerChannel(SimpleID::decode(query.value(0).toByteArray()), query.value(3).toString()));
  channel->setKey(id);
  channel->gender().setRaw(query.value(1).toLongLong());

  if (channel->type() == SimpleID::UserId) {
    channel->user()->set(user(id));

    Account account = DataBase::account(id);
    channel->setAccount(&account);
  }

  channel->setData(JSON::parse(query.value(4).toByteArray()).toMap());
  channel->setDate(query.value(5).toLongLong());
  FeedStorage::load(channel.data());

  return channel;
}


/*!
 * Получение ключа в таблице \b accounts на основе Сookie пользователя.
 */
qint64 DataBase::accountKey(const QByteArray &cookie)
{
  if (SimpleID::typeOf(cookie) != SimpleID::CookieId)
    return -1;

  QSqlQuery query;
  query.prepare(LS("SELECT id FROM accounts WHERE cookie = :cookie LIMIT 1;"));
  query.bindValue(LS(":cookie"), SimpleID::encode(cookie));
  query.exec();

  if (!query.first())
    return -1;

  return query.value(0).toLongLong();
}


/*!
 * Возвращает ключ в таблице \b channels на основе идентификатора канала или нормализированного имени.
 *
 * \param id   Идентификатор по которому будет производится поиск.
 * \param type Тип канала.
 *
 * \return Ключ в таблице или -1 в случае ошибки.
 *
 * \sa Normalize.
 * \sa SimpleID.
 */
qint64 DataBase::channelKey(const QByteArray &id, int type)
{
  int idType = SimpleID::typeOf(id);

  if (idType == SimpleID::CookieId) {
    qint64 key = accountKey(id);
    if (key == -1)
      return key;

    QSqlQuery query;
    query.prepare(LS("SELECT channel FROM accounts WHERE id = :id LIMIT 1;"));
    query.bindValue(LS(":id"), key);
    query.exec();

    if (!query.first())
      return -1;

    return query.value(0).toLongLong();
  }

  if (!Channel::isCompatibleId(id) && idType != SimpleID::NormalizedId)
    return -1;

  QSqlQuery query;

  if (idType == SimpleID::NormalizedId)
    query.prepare(LS("SELECT id FROM channels WHERE normalized = :id AND type = :type LIMIT 1;"));
  else
    query.prepare(LS("SELECT id FROM channels WHERE channel = :id AND type = :type LIMIT 1;"));

  query.bindValue(LS(":id"),   SimpleID::encode(id));
  query.bindValue(LS(":type"), type);
  query.exec();

  if (!query.first())
    return -1;

  return query.value(0).toLongLong();
}


/*!
 * Обновление схемы базы данных до версии 2.
 *
 * - В таблице \b accounts удаляются столбцы \b name и \b password и добавляются столбцы \b provider и \b data.
 * - В таблицу \b channels добавляется столбец date.
 */
qint64 DataBase::V2()
{
  QSqlQuery query;
  query.exec(LS("BEGIN TRANSACTION;"));
  query.exec(LS("ALTER TABLE accounts RENAME TO accounts_tmp;"));
  query.exec(LS(
    "CREATE TABLE IF NOT EXISTS accounts ( "
    "  id         INTEGER PRIMARY KEY,"
    "  channel    INTEGER UNIQUE,"
    "  date       INTEGER DEFAULT ( 0 ),"
    "  cookie     BLOB    NOT NULL UNIQUE,"
    "  provider   TEXT,"
    "  flags      INTEGER DEFAULT ( 0 ),"
    "  groups     TEXT,"
    "  data       BLOB"
    ");"
  ));

  query.exec(LS("INSERT INTO accounts (channel, date, cookie, groups) SELECT channel, date, cookie, groups FROM accounts_tmp;"));
  query.exec(LS("DROP TABLE accounts_tmp;"));
  query.exec(LS("ALTER TABLE channels ADD date INTEGER DEFAULT ( 0 )"));
  query.exec(LS("PRAGMA user_version = 2"));
  query.exec(LS("COMMIT;"));

  return 2;
}


/*!
 * Обновление схемы базы данных до версии 3.
 *
 * Права доступа к фидам \b acl принудительно изменяются с 744 на 766.
 */
qint64 DataBase::V3()
{
  QSqlQuery query;
  query.prepare(LS("SELECT id, json FROM feeds WHERE name = :name;"));
  query.bindValue(LS(":name"), FEED_NAME_ACL);
  query.exec();

  QMap<qint64, QByteArray> feeds;
  while (query.next())
    feeds[query.value(0).toLongLong()] = query.value(1).toByteArray().replace("\"mask\":484", "\"mask\":502").replace("\"mask\": 484", "\"mask\":502");

  query.exec(LS("BEGIN TRANSACTION;"));
  if (!feeds.isEmpty()) {
    query.prepare(LS("UPDATE feeds SET json = :json WHERE id = :id;"));

    QMapIterator<qint64, QByteArray> i(feeds);
    while (i.hasNext()) {
      i.next();
      query.bindValue(LS(":json"), i.value());
      query.bindValue(LS(":id"),   i.key());
      query.exec();
    }
  }

  query.exec(LS("PRAGMA user_version = 3"));
  query.exec(LS("COMMIT;"));

  return 3;
}


/*!
 * Обновление схемы базы данных до версии 4.
 */
qint64 DataBase::V4()
{
  // Обновление таблицы accounts.
  QSqlQuery query;
  query.exec(LS("BEGIN TRANSACTION;"));
  query.exec(LS("SELECT id, cookie FROM accounts"));

  QSqlQuery update;
  update.prepare(LS("UPDATE accounts SET cookie = :cookie WHERE id = :id;"));

  while (query.next()) {
    update.bindValue(LS(":id"),     query.value(0));
    update.bindValue(LS(":cookie"), SimpleID::encode(query.value(1).toByteArray()));
    update.exec();
  }

  query.exec(LS("COMMIT;"));

  // Обновление таблицы channels.
  query.exec(LS("BEGIN TRANSACTION;"));
  query.exec(LS("SELECT id, channel, normalized FROM channels"));
  update.prepare(LS("UPDATE channels SET channel = :channel, normalized = :normalized WHERE id = :id;"));

  while (query.next()) {
    update.bindValue(LS(":id"),         query.value(0));
    update.bindValue(LS(":channel"),    SimpleID::encode(query.value(1).toByteArray()));
    update.bindValue(LS(":normalized"), SimpleID::encode(query.value(2).toByteArray()));
    update.exec();
  }

  query.exec(LS("PRAGMA user_version = 4"));
  query.exec(LS("COMMIT;"));

  return 4;
}


/*!
 * Обновление информации об канале.
 * Если канал также содержит пользовательский аккаунт, то он также будет обновлён.
 */
void DataBase::update(ChatChannel channel)
{
  if (!channel)
    return;

  QSqlQuery query;
  query.prepare(LS("UPDATE channels SET channel = :channel, normalized = :normalized, type = :type, gender = :gender, name = :name, data = :data, date = :date WHERE id = :id;"));
  query.bindValue(LS(":channel"),    SimpleID::encode(channel->id()));
  query.bindValue(LS(":normalized"), SimpleID::encode(channel->normalized()));
  query.bindValue(LS(":type"),       channel->type());
  query.bindValue(LS(":gender"),     channel->gender().raw());
  query.bindValue(LS(":name"),       channel->name());
  query.bindValue(LS(":data"),       JSON::generate(channel->data()));
  query.bindValue(LS(":date"),       channel->date());
  query.bindValue(LS(":id"),         channel->key());
  query.exec();

  Account *account = channel->account();
  if (account && !account->saved) {
    query.prepare(LS("UPDATE accounts SET date = :date, cookie = :cookie, provider = :provider, flags = :flags, groups = :groups WHERE channel = :channel;"));
    query.bindValue(LS(":date"),       account->date);
    query.bindValue(LS(":cookie"),     SimpleID::encode(account->cookie));
    query.bindValue(LS(":provider"),   account->provider);
    query.bindValue(LS(":flags"),      account->flags);
    query.bindValue(LS(":groups"),     account->groups.toString());
    query.bindValue(LS(":channel"),    channel->key());
    query.exec();
  }
}


/*!
 * Добавление в базу информации о версии, в будущем эта информация может быть использована для автоматического обновления схемы базы данных.
 */
void DataBase::version()
{
  QSqlQuery query;

  query.exec(LS("PRAGMA user_version;"));
  if (!query.first())
    return;

  qint64 version = query.value(0).toLongLong();
  if (!version) {
    query.exec(LS("PRAGMA user_version = 4;"));
    version = 4;
  }

  query.finish();

  if (version == 1) version = V2();
  if (version == 2) version = V3();
  if (version == 3) version = V4();
}
