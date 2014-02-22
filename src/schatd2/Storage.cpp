/* $Id: Storage.cpp 3742 2013-07-10 02:22:58Z IMPOMEZIA $
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

#include <QFile>
#include <QSslConfiguration>
#include <QSslKey>
#include <QStringList>
#include <QTime>
#include <QUuid>

#if defined(Q_OS_UNIX)
#include <sys/resource.h>
#endif

#include "Ch.h"
#include "DataBase.h"
#include "DateTime.h"
#include "debugstream.h"
#include "feeds/FeedStorage.h"
#include "net/packets/auth.h"
#include "net/Protocol.h"
#include "net/SimpleID.h"
#include "NodeLog.h"
#include "Normalize.h"
#include "Path.h"
#include "Settings.h"
#include "SettingsHook.h"
#include "sglobal.h"
#include "Storage.h"
#include "StorageHook.h"

QStringList Storage::m_features;
Storage *Storage::m_self = 0;

const QString Storage::kLogColor  = QLatin1String("LogColor");
const QString Storage::kLogLevel  = QLatin1String("LogLevel");
const QString Storage::kLogOutput = QLatin1String("LogOutput");

#define LOG_N8010 LOG_INFO("N8010", "Core/Storage", "Max open files limit: " << limit.rlim_cur << " " << limit.rlim_max)
#define LOG_N8011 LOG_ERROR("N8011", "Core/Storage", "Unable to open log file: \"" << logFile << '"')

Storage::Storage(const QString &app, QObject *parent)
  : QObject(parent)
{
  m_self = this;
  qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()) ^ reinterpret_cast<quintptr>(this));

  Path::init(app);
  Normalize::init();

  new DataBase(this);
  new Ch(this);

  // Инициализация настроек по умолчанию.
  m_settings = new Settings(etc() + LC('/') + Path::app() + LS(".conf"), this);
  m_settings->setDefault(STORAGE_CERTIFICATE,    LS(":/server.crt"));
  m_settings->setDefault(STORAGE_LISTEN,         QStringList(LS("0.0.0.0:7667")));
  m_settings->setDefault(STORAGE_MAX_OPEN_FILES, 0);
  m_settings->setDefault(STORAGE_NICK_OVERRIDE,  true);
  m_settings->setDefault(STORAGE_PRIVATE_ID,     QString());
  m_settings->setDefault(STORAGE_PRIVATE_KEY,    LS(":/server.key"));
  m_settings->setDefault(STORAGE_WORKERS,        0);

  m_log = new NodeLog();
  new FeedStorage(this);

  add(new SettingsHook());
}


Storage::~Storage()
{
  qDeleteAll(m_hooks);
  delete m_log;
}


bool Storage::hasFeature(const QString &name)
{
  return m_features.contains(name);
}


QString Storage::var()
{
# if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
  if (!Path::isPortable())
    return LS("/var/lib/") + Path::app();
# endif

  return Path::cache();
}


void Storage::addFeature(const QString &name)
{
  if (!m_features.contains(name))
    m_features.append(name);
}


bool Storage::contains(const QString &key)
{
  if (m_self->m_cache.contains(key) || m_self->m_keys.contains(key) || DataBase::contains(key))
    return true;

  return false;
}

/*!
 * Запись данных в хранилище.
 *
 * \param key   Ключ в хранилище.
 * \param value Данные для записи в хранилище.
 */
int Storage::setValue(const QString &key, const QVariant &value)
{
  if (m_self->m_cache.value(key) == value)
    return Notice::NotModified;

  bool cache = true;
  StorageHook *hook = m_self->m_keys.value(key);
  if (hook) {
    if (!hook->setValue(key, value))
      return Notice::Forbidden;

    cache = hook->cache();
  }
  else
    DataBase::setValue(key, value);

  if (cache) {
    m_self->m_cache[key] = value;

    if (key == kLogLevel)
      NodeLog::setLevel(static_cast<NodeLog::Level>(value.toInt()));
    else if (key == kLogOutput)
      NodeLog::setOutFlags(static_cast<NodeLog::OutFlags>(value.toInt()));
    else if (key == kLogColor)
      NodeLog::setColors(value.toBool());

    emit m_self->valueChanged(key, value);
  }

  return Notice::OK;
}


/*!
 * Получение данных из хранилища.
 *
 * \param key          Ключ в хранилище.
 * \param defaultValue Значение по умолчанию.
 */
QVariant Storage::value(const QString &key, const QVariant &defaultValue)
{
  if (m_self->m_cache.contains(key))
    return m_self->m_cache.value(key);

  bool cache = true;
  StorageHook *hook = m_self->m_keys.value(key);
  QVariant value;

  if (hook) {
    value = hook->value(key, defaultValue);
    cache = hook->cache();
  }
  else
    value = DataBase::value(key, defaultValue);

  if (cache)
    m_self->m_cache[key] = value;

  return value;
}


/*!
 * Добавление хука.
 */
void Storage::add(StorageHook *hook)
{
  if (!hook)
    return;

  QStringList keys = hook->keys();
  if (keys.isEmpty()) {
    delete hook;
    return;
  }

  m_self->m_hooks.append(hook);
  foreach (const QString &key, keys) {
    m_self->m_keys[key] = hook;
  }
}


int Storage::load()
{
  Ch::load();

  return 0;
}


/*!
 * Запуск сервера, функция производит инициализацию состояния и объектов сервера.
 */
int Storage::start()
{
  QString logFile = Path::cache();
# if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
  if (!Path::isPortable())
    logFile = LS("/var/log/") + Path::app();
# endif
  logFile += LC('/') + Path::app() + LS(".log");

  if (!m_log->open(logFile, NodeLog::ErrorLevel)) {
    LOG_N8011
  }

  setDefaultSslConf();

# if defined(Q_OS_UNIX)
  setMaxOpenFiles(m_settings->value(STORAGE_MAX_OPEN_FILES).toInt());
# endif

  if (DataBase::start())
    return 1;

  NodeLog::setLevel(static_cast<NodeLog::Level>(value(kLogLevel, NodeLog::ErrorLevel).toInt()));
  NodeLog::setOutFlags(static_cast<NodeLog::OutFlags>(value(kLogOutput, NodeLog::FileOut).toInt()));
  NodeLog::setColors(value(kLogColor, true).toBool());

  m_privateId = m_settings->value(STORAGE_PRIVATE_ID).toString().toUtf8();
  if (m_privateId.isEmpty()) {
    m_privateId = value(STORAGE_PRIVATE_ID).toByteArray();

    if (m_privateId.isEmpty())
      m_privateId = ChatId(ChatId::UniqueUserId).toBase32();
  }

  m_id = SimpleID::make(m_privateId, SimpleID::ServerId);
  setValue(STORAGE_PRIVATE_ID, m_privateId);
  return 0;
}


QString Storage::etc() const
{
# if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
  if (!Path::isPortable())
    return LS("/etc/") + Path::app();
# endif

  return Path::data();
}


void Storage::setDefaultSslConf()
{
# if !defined(SCHAT_NO_SSL)
  if (!QSslSocket::supportsSsl())
    return;

  QList<QSslCertificate> certificates = QSslCertificate::fromPath(Path::file(etc(), m_settings->value(STORAGE_CERTIFICATE).toString()));
  if (certificates.isEmpty())
    return;

  QSslConfiguration conf = QSslConfiguration::defaultConfiguration();

  QFile key(Path::file(etc(), m_settings->value(STORAGE_PRIVATE_KEY).toString()));
  if (key.exists() && key.open(QFile::ReadOnly)) {
    conf.setPrivateKey(QSslKey(&key, QSsl::Rsa));
    key.close();
  }

  conf.setLocalCertificate(certificates.takeFirst());

  if (!certificates.isEmpty()) {
    QList<QSslCertificate> ca = conf.caCertificates();
    foreach (const QSslCertificate &cert, certificates) {
      ca.append(cert);
    }

    conf.setCaCertificates(ca);
  }

# if QT_VERSION >= 0x050000
  conf.setProtocol(QSsl::TlsV1_0);
# else
  conf.setProtocol(QSsl::TlsV1);
# endif

  QSslConfiguration::setDefaultConfiguration(conf);
# endif
}


void Storage::setMaxOpenFiles(int max)
{
# if defined(Q_OS_UNIX)
  struct rlimit limit;

  if (max > 0) {
    limit.rlim_cur = max;
    limit.rlim_max = max;

    setrlimit(RLIMIT_NOFILE, &limit);
  }

  if (getrlimit(RLIMIT_NOFILE, &limit) == 0) {
    LOG_N8010
  }
# else
  Q_UNUSED(max)
# endif
}
