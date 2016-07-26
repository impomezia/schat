/* Simple Chat
 * Copyright (c) 2008-2016 Alexander Sedov <imp@schat.me>
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

#ifndef NODEMESSAGESDB_H_
#define NODEMESSAGESDB_H_

#include <QMutex>
#include <QObject>
#include <QRunnable>

#include "net/packets/MessageNotice.h"

class MessageNotice;
class QSqlQuery;

class NodeMessagesDB : public QObject
{
  Q_OBJECT

public:
  NodeMessagesDB(QObject *parent = 0);
  ~NodeMessagesDB();
  static bool open();
  static int status(int status);
  static QList<ChatId> last(const ChatId &channel, const int limit, const qint64 before);
  static QList<ChatId> last(const ChatId &user1, const ChatId &user2, const int limit, const qint64 before);
  static QList<ChatId> since(const ChatId &channel, const qint64 start, const qint64 end);
  static QList<ChatId> since(const ChatId &user1, const ChatId &user2, const qint64 start, const qint64 end);
  static QList<MessageRecordV2> get(const QList<ChatId> &ids, const ChatId &userId);
  static QList<MessageRecordV2> offline(const ChatId &userId);
  static void add(const MessageNotice &packet, int status = 300);
  static void markAsRead(const QList<MessageRecordV2> &records);
  static void remove(qint64 id);
  static void update(const MessageRecordV2 &record);

private slots:
  void startTasks();

private:
  /// Кэш каналов.
  class ChannelsCache
  {
  public:
    ChannelsCache() {}

    ChatId get(qint64 key);
    qint64 get(const ChatId &id);

  private:
    qint64 add(const ChatId &id, qint64 value);

    QMap<ChatId, qint64> m_forward;  ///< Прямая таблица поиска для преобразования идентификатора канала в id в базе.
    QMap<qint64, ChatId> m_backward; ///< Обратная таблица поиска для преобразования id в базе в идентификатор канала.
    QMutex m_mutex;                  ///< Мьютекс для защиты доступа к данным.
  };

  friend class AddMessageTask;
  friend class UpdateMessageTask;

  static qint64 V2();
  static qint64 V3();
  static qint64 V4();
  static qint64 V5();
  static QList<ChatId> ids(QSqlQuery &query);
  static QList<MessageRecordV2> messages(QSqlQuery &query);
  static void version();

  ChannelsCache m_channels;      ///< Кэш каналов.
  QList<QRunnable*> m_tasks;     ///< Задачи для выполнения в отдельном потоке.
  static NodeMessagesDB *m_self; ///< Указатель на себя.
  static QString m_id;           ///< Идентификатор сооединения с базой, это строка всегда равна "messages".
};


class AddMessageTask : public QRunnable
{
public:
  /// Формат данных в таблице storage.
  enum Format {
    AutoFormat,    ///< Автоматическое определение формата.
    JSonFormat,    ///< JSON формат.
  };

  inline AddMessageTask(const MessageNotice &packet, int status = 300) : QRunnable()
  , m_status(status)
  , m_packet(packet)
  {}

  void run();

private:
  const int m_status;           ///< Статус сообщения.
  const MessageNotice m_packet; ///< Копия пакета.
};


class UpdateMessageTask : public QRunnable
{
public:
  inline UpdateMessageTask(const MessageRecordV2 &record) : QRunnable()
  , m_record(record)
  {}

  void run();

private:
  const MessageRecordV2 m_record;
};

#endif /* NODEMESSAGESDB_H_ */
