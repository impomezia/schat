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

#ifndef HISTORYDB_H_
#define HISTORYDB_H_

#include <QObject>
#include <QRunnable>

#include "feeds/Feed.h"
#include "net/packets/MessageNotice.h"

class MessageId;

/*!
 * Интерфейс базы данных локальной копии истории сообщений.
 */
class HistoryDB : public QObject
{
  Q_OBJECT

public:
  HistoryDB(QObject *parent = 0);
  ~HistoryDB();

  inline static QString id() { return m_id; }
  static bool open(const QByteArray &id, const QString &dir);
  static int status(int status);
  static MessageRecord get(const ChatId &id);
  static QList<QByteArray> last(const QByteArray &channelId);
  static QString tag(const QByteArray &channelId);
  static void add(const QByteArray &channelId, const QStringList &messages);
  static void add(MessagePacket packet);
  static void clear();
  static void close();

private slots:
  void startTasks();

private:
  static void create();
  static void version();

  static qint64 V2();
  static qint64 V3();
  static qint64 V4();

  QList<QRunnable*> m_tasks; ///< Задачи для выполнения в отдельном потоке.
  static HistoryDB *m_self;  ///< Указатель на себя.
  static QString m_id;       ///< Идентификатор соединения с базой.
};


namespace history {

/*!
 * Отложенная операция добавления сообщения в базу данных.
 */
class AddMessage : public QRunnable
{
public:
  AddMessage(MessagePacket packet);
  void run();

private:
  MessageNotice m_packet;
};


/*!
 * Отложенная операция добавления или обновления списка последних сообщений для канала.
 */
class AddLast : public QRunnable
{
public:
  AddLast(const QByteArray &channelId, const QStringList &messages);
  void run();

private:
  const QByteArray m_channelId;
  const QStringList m_messages;
};

} /* namespace history */

#endif /* HISTORYDB_H_ */
