/* $Id: NodeWorker.h 2137 2012-01-12 14:05:41Z IMPOMEZIA $
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

#ifndef NODEWORKER_H_
#define NODEWORKER_H_

#include <QHash>
#include <QReadWriteLock>
#include <QThread>

#include "schat.h"

class NewConnectionEvent;
class NewPacketsEvent;
class NodeWorkerListener;
class SimpleSocket;

class SCHAT_EXPORT NodeWorker : public QThread
{
  Q_OBJECT

public:
  NodeWorker(QObject *core);

signals:
  void ready(NodeWorkerListener *listener);

protected:
  void run();

private:
  QObject *m_core; ///< Указатель на ядро чата.
};


class NodeWorkerListener : public QObject
{
  Q_OBJECT

public:
  NodeWorkerListener(QObject *core);
  ~NodeWorkerListener();
  int count();

protected:
  void customEvent(QEvent *event);

private slots:
  void packets(quint64 id, const QList<QByteArray> &packets);
  void released(quint64 id);

private:
  void add(NewConnectionEvent *event);
  void packets(NewPacketsEvent *event);

  QHash<quint64, SimpleSocket*> m_sockets; ///< Таблица сокетов.
  QObject *m_core;                         ///< Указатель на ядро чата.
  QReadWriteLock m_lock;                   ///< Защищает доступ к \p m_sockets.
};

#endif /* NODEWORKER_H_ */
