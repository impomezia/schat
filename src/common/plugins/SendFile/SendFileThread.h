/* $Id: SendFileThread.h 2772 2012-06-13 03:25:48Z IMPOMEZIA $
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

#ifndef SENDFILETHREAD_H_
#define SENDFILETHREAD_H_

#include <QThread>
#include <QVariant>

#include "SendFileTransaction.h"

namespace SendFile {

class Worker;

/*!
 * Рабочий поток передачи файлов.
 */
class Thread : public QThread
{
  Q_OBJECT

public:
  Thread(quint16 port, QObject *parent = 0);
  bool setPort(quint16 port);
  inline Worker *worker() const { return m_worker; }
  void add(SendFileTransaction transaction);
  void remove(const QByteArray &id);

signals:
  void addTask(const QVariantMap &data);
  void finished(const QByteArray &id, qint64 elapsed);
  void progress(const QByteArray &id, qint64 current, qint64 total, int percent);
  void ready();
  void removeTask(const QByteArray &id);
  void started(const QByteArray &id, qint64 time);

protected:
  void run();

private slots:
  void workerReady();

private:
  QList<SendFileTransaction> m_pending; ///< Транзацкции ожидающие запуска потока.
  quint16 m_port;                       ///< Порт который будет использоватся для подключений.
  Worker *m_worker;                     ///< Объект выполняющий работу потока.
};

} // namespace SendFile

#endif /* SENDFILETHREAD_H_ */
