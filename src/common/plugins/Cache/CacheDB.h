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

#ifndef CACHEDB_H_
#define CACHEDB_H_

#include <QMutex>
#include <QObject>

#include "Channel.h"

class QRunnable;

/*!
 * Интерфейс базы данных плагина \b Cache.
 */
class CacheDB : public QObject
{
  Q_OBJECT

public:
  CacheDB(QObject *parent = 0);
  ~CacheDB();

  inline static QString id() { return m_id; }
  static bool open(const QByteArray &id, const QString &dir);
  static ClientChannel channel(const QByteArray &id, bool feeds = true);
  static qint64 key(const QByteArray &id);
  static void add(ClientChannel channel);
  static void add(FeedPtr feed);
  static void clear();

private slots:
  void start();

private:
  static ClientChannel channel(qint64 id, bool feeds = true);
  static qint64 V2();
  static qint64 V3();
  static void close();
  static void create();
  static void setKey(const QByteArray &id, qint64 key);
  static void version();

  QList<QRunnable*> m_tasks;        ///< Задачи для выполнения в отдельном потоке.
  QMap<QByteArray, qint64> m_cache; ///< Кеш каналов, ключ в таблице идентификатор канала, значение первичный ключ в таблице channels.
  QMutex m_mutex;                   ///< Мьютекс защищающий доступ к кешу каналов.
  static CacheDB *m_self;           ///< Указатель на себя.
  static QString m_id;              ///< Идентификатор соединения с базой.
};

#endif /* CACHEDB_H_ */
