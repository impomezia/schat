/* $Id: NodePool.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QDebug>

#include <QCoreApplication>

#include "events.h"
#include "net/NodePool.h"
#include "net/NodeWorker.h"
#include "net/TcpServer.h"

NodePool::NodePool(const QStringList &listen, int workers, QObject *core)
  : QThread()
  , m_count(workers)
  , m_core(core)
  , m_listen(listen)
  , m_counter(0)
{
  if (m_count <= 0) {
    m_count = QThread::idealThreadCount() - 1;
    if (m_count == 0)
      m_count = 1;
  }
}


NodePool::~NodePool()
{
  foreach (TcpServer *server, m_servers) {
    server->close();
    delete server;
  }

  foreach (NodeWorker *worker, m_workers) {
    worker->quit();
  }

  foreach (NodeWorker *worker, m_workers) {
    worker->wait();
    delete worker;
  }
}


void NodePool::run()
{
  for (int i = 0; i < m_count; ++i) {
    NodeWorker *worker = new NodeWorker(m_core);
    m_workers.append(worker);
    connect(worker, SIGNAL(ready(NodeWorkerListener*)), SLOT(workerReady(NodeWorkerListener*)));

    worker->start();
  }

  foreach (QString host, m_listen) {
    TcpServer *server = new TcpServer;
    m_servers.append(server);
    connect(server, SIGNAL(newConnection(int)), SLOT(newConnection(int)), Qt::DirectConnection);
    server->listen(host);
  }

  exec();
}


/*!
 * Обработка нового подключения.
 */
void NodePool::newConnection(int socketDescriptor)
{
  if (m_listeners.isEmpty())
    return;

  m_counter++;
  QMap<int, NodeWorkerListener*> map;
  foreach (NodeWorkerListener *listener, m_listeners) {
    map[listener->count()] = listener;
  }

  QCoreApplication::postEvent(map.value(map.keys().at(0)), new NewConnectionEvent(socketDescriptor, m_counter));
}


/*!
 * Обработка готовности потока NodeWorker.
 */
void NodePool::workerReady(NodeWorkerListener *listener)
{
  m_listeners.append(listener);
  emit ready(listener);
}
