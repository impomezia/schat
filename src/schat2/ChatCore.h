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

#ifndef CHATCORE_H_
#define CHATCORE_H_

#include <QObject>
#include <QUrl>
#include <QVariant>

#include "schat.h"

class ChatClient;
class ChatPlugins;
class ChatSettings;
class Extensions;
class NetworkAccess;
class NetworkManager;
class QThreadPool;
class ServiceThread;
class SimpleClient;
class Translation;

#define SCHAT_OPTION(x) ChatCore::settings()->value(x)

class SCHAT_CORE_EXPORT ChatCore : public QObject
{
  Q_OBJECT

public:
  ChatCore(QObject *parent = 0);
  ~ChatCore();
  inline static bool isReady()                          { return m_ready; }
  inline static ChatPlugins *plugins()                  { return i()->m_plugins; }
  inline static ChatSettings *settings()                { return i()->m_settings; }
  inline static Extensions *extensions()                { return i()->m_extensions; }
  inline static NetworkAccess *networkAccess()          { return i()->m_networkAccess; }
  inline static NetworkManager *networks()              { return i()->m_networkManager; }
  inline static QByteArray currentId()                  { return i()->m_currentId; }
  inline static QThreadPool *pool()                     { return i()->m_pool; }
  inline static ServiceThread *service()                { return i()->m_service; }
  inline static Translation *translation()              { return i()->m_translation; }
  inline static void setCurrentId(const QByteArray &id) { i()->m_currentId = id; }
  static ChatCore *i();
  static QByteArray randomId();
  static QStringList config();

signals:
  void ready();

public slots:
  void send(const QString &text);

private slots:
  void onReady();
  void onSettingsChanged(const QString &key, const QVariant &value);
  void open();

private:
  void loadTranslation();

  ChatClient *m_client;             ///< Клиент чата.
  ChatPlugins *m_plugins;           ///< Загрузчик плагинов.
  ChatSettings *m_settings;         ///< Настройки.
  Extensions *m_extensions;         ///< Загрузчик расширений.
  NetworkAccess *m_networkAccess;   ///< Модуль доступа к сети.
  NetworkManager *m_networkManager; ///< Объект управляющих сетями.
  QByteArray m_currentId;           ///< Идентификатор текущей вкладки.
  QThreadPool *m_pool;              ///< Пул для запуска потоков.
  ServiceThread *m_service;         ///< Сервисный поток для выполнения фоновых задач.
  static bool m_ready;              ///< true после полной инициализации.
  static ChatCore *m_self;          ///< Указатель на себя.
  Translation *m_translation;       ///< Модуль загрузки переводов.
};

#endif /* CHATCORE_H_ */
