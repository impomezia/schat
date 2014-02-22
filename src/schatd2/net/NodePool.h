/* $Id: NodePool.h 2136 2012-01-12 11:34:16Z IMPOMEZIA $
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

#ifndef NODEPOOL_H_
#define NODEPOOL_H_

#include <QStringList>
#include <QThread>

#include "schat.h"

class NodeWorker;
class NodeWorkerListener;
class TcpServer;

/*!
 * Пул потоков сервера, обрабатывающих подключения.
 */
class SCHAT_EXPORT NodePool : public QThread
{
  Q_OBJECT

public:
  NodePool(const QStringList &listen, int workers, QObject *core);
  ~NodePool();

signals:
  void ready(QObject *listener);

protected:
  void run();

private slots:
  void newConnection(int socketDescriptor);
  void workerReady(NodeWorkerListener *listener);

private:
  int m_count;                             ///< Число потоков.
  QList<NodeWorker *> m_workers;           ///< Список потоков обслуживающих подключения.
  QList<NodeWorkerListener *> m_listeners; ///< Указатели на объекты слушатели потоков обработки сообщений.
  QList<TcpServer *> m_servers;            ///< Список серверов.
  QObject *m_core;                         ///< Указатель на ядро чата.
  QStringList m_listen;                    ///< Список пар адресов и портов на которых сервер будет принимать подключения.
  quint64 m_counter;                       ///< Счётчик, определяющий глобальный идентификатор сокета.
};

#endif /* NODEPOOL_H_ */
