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

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QThread>

#include "ServiceList.h"
#include "interfaces/IServiceTask.h"

ServiceList::ServiceList()
  : QObject()
{
  m_net = new QNetworkAccessManager(this);
  m_net->get(QNetworkRequest())->deleteLater();
}


void ServiceList::append(IServiceTask *task)
{
  Q_ASSERT(QThread::currentThread() == thread());
  SLOG_DEBUG(QThread::currentThread() << "counter =" << task->counter() << "task =" << task);

  QObject *object = task->create(m_net, this);
  m_tasks.append(object);

  const qint64 counter = task->counter();

  delete task;
  emit ready(counter, object);
}


void ServiceList::append(QQueue<IServiceTask*> queue)
{
  while (!queue.isEmpty())
    append(queue.dequeue());
}
