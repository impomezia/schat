/* $Id: SendFileThread.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#include "SendFileThread.h"
#include "SendFileWorker.h"

namespace SendFile {

Thread::Thread(quint16 port, QObject *parent)
  : QThread(parent)
  , m_port(port)
  , m_worker(0)
{
  connect(this, SIGNAL(ready()), SLOT(workerReady()));
}


/*!
 * Установка порта.
 *
 * Если поток уже запущен, установка порта будет невозможна и функция вернёт \b false.
 */
bool Thread::setPort(quint16 port)
{
  if (m_worker || isRunning())
    return false;

  m_port = port;
  return true;
}


/*!
 * Добавление транзакции на исполнение.
 *
 * В случае если поток не был запущен, инициализируется его запуск и транзакция добавляется в очередь ожидающую запуск потока.
 */
void Thread::add(SendFileTransaction transaction)
{
  if (!isRunning()) {
    m_pending.append(transaction);
    start();
    return;
  }

  emit addTask(transaction->toMap());
}


void Thread::remove(const QByteArray &id)
{
  emit removeTask(id);
}


void Thread::run()
{
  m_worker = new Worker(m_port);

  emit ready();
  exec();
}


void Thread::workerReady()
{
  connect(this, SIGNAL(addTask(QVariantMap)), m_worker, SLOT(addTask(QVariantMap)));
  connect(this, SIGNAL(removeTask(QByteArray)), m_worker, SLOT(removeTask(QByteArray)));
  connect(m_worker, SIGNAL(finished(QByteArray,qint64)), SIGNAL(finished(QByteArray,qint64)));
  connect(m_worker, SIGNAL(progress(QByteArray,qint64,qint64,int)), SIGNAL(progress(QByteArray,qint64,qint64,int)));
  connect(m_worker, SIGNAL(started(QByteArray,qint64)), SIGNAL(started(QByteArray,qint64)));

  while (!m_pending.isEmpty())
    emit addTask(m_pending.takeFirst()->toMap());
}

} // namespace SendFile
