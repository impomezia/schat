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

#ifndef DATABASE_H_
#define DATABASE_H_

#include <QObject>
#include <QRunnable>

#include "net/SimpleID.h"
#include "ServerChannel.h"

class QThreadPool;

/*!
 * База данных сервера.
 */
class SCHAT_EXPORT DataBase : public QObject
{
  Q_OBJECT

public:
  DataBase(QObject *parent = 0);
  ~DataBase();
  inline static QThreadPool *pool() { return m_self->m_pool; }
  static int start();

  // channels.
  static ChatChannel channel(const QByteArray &id, int type = SimpleID::ChannelId);
  static qint64 add(ChatChannel channel);
  static qint64 isCollision(const QByteArray &id, const QByteArray &normalized);
  static qint64 isCollision(const QByteArray &id, const QByteArray &normalized, int type);
  static void saveData(Channel *channel);

  // hosts.
  static QMap<ChatId, HostInfo> hosts(qint64 channel);
  static void add(HostInfo host);
  static void removeHost(const QByteArray &hostId);

  static bool contains(const QString &key);
  static QVariant value(const QString &key, const QVariant &defaultValue = QVariant());
  static void setValue(const QString &key, const QVariant &value);

private slots:
  void startTasks();

private:
  static ChatChannel channel(qint64 id);
  static qint64 channelKey(const QByteArray &id, int type = SimpleID::ChannelId);
  static qint64 V2();
  static qint64 V3();
  static qint64 V4();
  static qint64 V5();
  static void update(ChatChannel channel);
  static void version();

  QList<QRunnable*> m_tasks; ///< Задачи для выполнения в отдельном потоке.
  QThreadPool *m_pool;       ///< Пул для запуска потоков.
  static DataBase *m_self;   ///< Указатель на себя.
};


/*!
 * Отложенная запись или обновление информации о хосте пользователя.
 */
class AddHostTask : public QRunnable
{
public:
  AddHostTask(Host *host);
  void run();

private:
  Host m_host; ///< Информация о хосте пользователя.
};


class AddValueTask : public QRunnable
{
public:
  /// Формат данных в таблице storage.
  enum Format {
    AutoFormat,    ///< Автоматическое определение формата.
    JSonFormat,    ///< JSON формат.
  };

  AddValueTask(const QString &key, const QVariant &value);
  void run();

private:
  QString m_key;
  QVariant m_value;
};

#endif /* DATABASE_H_ */
