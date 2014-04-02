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

#ifndef STORAGE_H_
#define STORAGE_H_

#include <QByteArray>
#include <QHash>

#include "net/SimpleID.h"
#include "ServerChannel.h"

class NodeLog;
class ServerData;
class Settings;
class StorageHook;

#define STORAGE_CERTIFICATE    QLatin1String("Certificate")
#define STORAGE_LISTEN         QLatin1String("Listen")
#define STORAGE_MAX_OPEN_FILES QLatin1String("MaxOpenFiles")
#define STORAGE_NICK_OVERRIDE  QLatin1String("NickOverride")
#define STORAGE_PASSWORD       QLatin1String("password")
#define STORAGE_PEAK_USERS     QLatin1String("PeakUsers")
#define STORAGE_PRIVATE_ID     QLatin1String("PrivateId")
#define STORAGE_PRIVATE_KEY    QLatin1String("PrivateKey")
#define STORAGE_WORKERS        QLatin1String("Workers")

class SCHAT_EXPORT Storage : public QObject
{
  Q_OBJECT

public:
  static const QString kLogColor;
  static const QString kLogLevel;
  static const QString kLogOutput;

  Storage(Settings *settings, const QString &app, QObject *parent = 0);
  ~Storage();
  inline static QByteArray privateId()     { return m_self->m_privateId; }
  inline static QByteArray serverId()      { return m_self->m_id; }
  inline static Settings *settings()       { return m_self->m_settings; }
  inline static Storage *i()               { return m_self; }
  static bool hasFeature(const QString &name);
  static QString var();
  static void addFeature(const QString &name);
  static bool contains(const QString &key);
  static int setValue(const QString &key, const QVariant &value);
  static QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
  static void add(StorageHook *hook);

  int load();
  int start();

signals:
  void valueChanged(const QString &key, const QVariant &value);

private:
  QString etc() const;
  void setDefaultSslConf();
  void setMaxOpenFiles(int max);

  NodeLog *m_log;                      ///< Журнал.
  QByteArray m_id;                     ///< Публичный идентификатор сервера.
  QByteArray m_privateId;              ///< Приватный идентификатор сервера.
  QList<StorageHook *> m_hooks;        ///< Хуки хранилища.
  QMap<QString, QVariant> m_cache;     ///< Кеш хранилища.
  QMap<QString, StorageHook *> m_keys; ///< Ключи значений хранилища обрабатываемые хуками.
  ServerData *m_serverData;            ///< Информация о сервере.
  Settings *m_settings;                ///< Настройки сервера.
  static QStringList m_features;       ///< Список дополнительных API.
  static Storage *m_self;              ///< Указатель на себя.
};

#endif /* STORAGE_H_ */
