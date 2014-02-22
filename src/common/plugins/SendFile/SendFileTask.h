/* $Id: SendFileTask.h 2777 2012-06-16 08:00:40Z IMPOMEZIA $
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

#ifndef SENDFILETASK_H_
#define SENDFILETASK_H_

#include <QHostAddress>
#include <QObject>
#include <QSharedPointer>
#include <QVariant>

class QBasicTimer;
class QFile;
class Socket;

namespace SendFile {

class Transaction;
class Worker;

class Task : public QObject
{
  Q_OBJECT

public:
  Task(const QVariantMap &data);
  ~Task();
  bool handshake(Socket *socket, char role);
  bool init();
  bool isReadyToRemove() const;
  inline Socket *socket() const           { return m_socket; }
  inline Transaction *transaction() const { return m_transaction; }
  inline void resetSocket()               { m_socket = 0; }
  void discovery();
  void stop();

signals:
  void finished(const QByteArray &id, qint64 elapsed);
  void progress(const QByteArray &id, qint64 current, qint64 total, int percent);
  void released(const QByteArray &id);
  void started(const QByteArray &id, qint64 time);

protected:
  void timerEvent(QTimerEvent *event);

private slots:
  void accepted();
  void acceptRequest();
  void finished();
  void progress(qint64 current);
  void released();
  void syncRequest();

private:
  void discovery(const QString &host, quint16 port);
  void start();
  void stopDiscovery();

  bool m_finished;             ///< \b true если задача завершена и ожидает отключения всех сокетов.
  QBasicTimer *m_timer;        ///< Таймер для обновления прогресса загрузки.
  QFile *m_file;               ///< Открытый файл.
  qint64 m_pos;                ///< Текущая отправленная или полученная позиция.
  qint64 m_time;               ///< Время начала передачи файла.
  QList<Socket *> m_discovery; ///< Список сокетов находящихся в состоянии поиска.
  Socket *m_socket;            ///< Сокет для передачи данных.
  Transaction *m_transaction;  ///< Транзакция.
};

} // namespace SendFile

typedef QSharedPointer<SendFile::Task> SendFileTask;

#endif /* SENDFILETASK_H_ */
