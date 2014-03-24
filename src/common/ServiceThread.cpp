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

#include <QTime>
#include <QMetaType>

#include "ServiceThread.h"
#include "interfaces/IServiceTask.h"
#include "ServiceList.h"

ServiceThread::ServiceThread(QObject *parent)
  : QThread(parent)
  , m_ready(false)
  , m_counter(-1)
{
  qRegisterMetaType<IServiceTask*>("IServiceTask*");
  qRegisterMetaType<QQueue<IServiceTask*> >("QQueue<IServiceTask*>");

  connect(this, SIGNAL(ready()), SLOT(onReady()));
}


qint64 ServiceThread::add(IServiceTask *task)
{
  Q_ASSERT(task);

  SLOG_DEBUG(currentThread() << "task =" << task);

  task->setCounter(++m_counter);

  if (!isReady()) {
    m_mutex.lock();
    m_queue.enqueue(task);
    m_mutex.unlock();
  }
  else
    QMetaObject::invokeMethod(m_list, "append", Qt::QueuedConnection, Q_ARG(IServiceTask*, task));

  return m_counter;
}


void ServiceThread::run()
{
  SCHAT_DEBUG_CODE(QTime t; t.start());

  m_list = new ServiceList();
  connect(m_list, SIGNAL(ready(qint64,QObject*)), SIGNAL(ready(qint64,QObject*)));

  SLOG_DEBUG("elapsed =" << t.elapsed() << "ms");

  m_mutex.lock();
  if (!m_queue.isEmpty()) {
    m_list->append(m_queue);
    m_queue.clear();
  }
  m_mutex.unlock();

  emit ready();
  exec();

  SLOG_DEBUG("tasks =" << m_list->size());

  delete m_list;
}


void ServiceThread::onReady()
{
  m_ready = true;

  QMutexLocker locker(&m_mutex);

  if (m_queue.isEmpty())
    return;

  QMetaObject::invokeMethod(m_list, "append", Qt::QueuedConnection, Q_ARG(QQueue<IServiceTask*>, m_queue));
  m_queue.clear();
}
