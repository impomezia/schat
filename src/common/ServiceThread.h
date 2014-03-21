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

#ifndef SERVICETHREAD_H_
#define SERVICETHREAD_H_

#include <QMutex>
#include <QQueue>
#include <QThread>

#include "schat.h"

class QNetworkAccessManager;
class IServiceTask;
class ServiceList;

/*!
 * Сервисный поток для выполнения фоновых задач.
 */
class SCHAT_EXPORT ServiceThread : public QThread
{
  Q_OBJECT

public:
  ServiceThread(QObject *parent = 0);
  qint64 add(IServiceTask *task);
  inline bool isReady() const { return m_ready; }

signals:
  void ready();
  void ready(qint64 counter, QObject *object);

protected:
  void run() Q_DECL_OVERRIDE;

private slots:
  void onReady();

private:
  bool m_ready;                  ///< true если поток успешно запущен.
  qint64 m_counter;              ///< Счётчик задач.
  QMutex m_mutex;
  QQueue<IServiceTask*> m_queue; ///< Очередь ожидания запуска потока.
  ServiceList *m_list;           ///< Список сервисных задач.
};

#endif // SERVICETHREAD_H_
