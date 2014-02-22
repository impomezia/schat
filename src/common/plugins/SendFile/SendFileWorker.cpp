/* $Id: SendFileWorker.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QTimer>
#include <QTimerEvent>

#include "debugstream.h"
#include "net/SimpleID.h"
#include "SendFileSocket.h"
#include "SendFileTask.h"
#include "SendFileWorker.h"
#include "sglobal.h"

namespace SendFile {

Worker::Worker(quint16 port, QObject *parent)
  : QTcpServer(parent)
  , m_port(port)
{
  m_timer = new QBasicTimer();

  start();

  SCHAT_DEBUG_STREAM("[SendFile] Worker::Worker(), port:" << port);
# if defined(SCHAT_DEBUG)
  if (!isListening())
    qDebug() << "        ERROR:" << errorString();
# endif
}


Worker::~Worker()
{
  if (m_timer->isActive())
    m_timer->stop();

  delete m_timer;
}


/*!
 * Добавление задачи.
 */
void Worker::addTask(const QVariantMap &data)
{
  SCHAT_DEBUG_STREAM("[SendFile] Worker::add(), id:" << SimpleID::encode(data.value("id").toByteArray()));
  start();

  QByteArray id = data.value("id").toByteArray();
  if (m_tasks.contains(id)) {
    updateTask(id, data);
    return;
  }

  SendFileTask task(new Task(data));
  if (!task->transaction()->isValid())
    return;

  if (!task->init())
    return;

  connect(task.data(), SIGNAL(finished(QByteArray,qint64)), SIGNAL(finished(QByteArray,qint64)));
  connect(task.data(), SIGNAL(released(QByteArray)), SLOT(removeTask(QByteArray)));
  connect(task.data(), SIGNAL(progress(QByteArray,qint64,qint64,int)), SIGNAL(progress(QByteArray,qint64,qint64,int)));
  connect(task.data(), SIGNAL(started(QByteArray,qint64)), SIGNAL(started(QByteArray,qint64)));
  m_tasks[id] = task;
}


/*!
 * Удаление задачи.
 *
 * Удаление откладывается до следующего цикла обработки событий из-за высокой вероятности падения при удалении сокетов.
 */
void Worker::removeTask(const QByteArray &id)
{
  SCHAT_DEBUG_STREAM("[SendFile] Worker::removeTask(), id:" << SimpleID::encode(id));

  SendFileTask task = m_tasks.value(id);
  if (!task)
    return;

  if (!m_remove.contains(id)) {
    task->stop();
    m_remove.append(id);
  }
  else if (task->isReadyToRemove()) {
    m_remove.removeAll(id);
    m_tasks.remove(id);
  }
}


/*!
 * Обработка запроса на авторизацию от входящего подключения.
 */
void Worker::handshake(const QByteArray &id, char role)
{
  Socket *socket = qobject_cast<Socket*>(sender());
  if (!socket)
    return;

  SCHAT_DEBUG_STREAM("[SendFile] Worker::handshake()" << socket->peerAddress().toString() << "socket:" << socket);
  if (!m_tasks.contains(id))
    socket->reject();

  SendFileTask task = m_tasks.value(id);
  if (!task || !task->handshake(socket, role))
    socket->reject();
}


/*!
 * Удаление сокета при отключении.
 */
void Worker::released()
{
  Socket *socket = qobject_cast<Socket*>(sender());
  if (!socket)
    return;

  SCHAT_DEBUG_STREAM("[SendFile] Worker::released(), socket:" << socket);
  if (!socket->id().isEmpty()) {
    SendFileTask task = m_tasks.value(socket->id());
    if (task && task->socket() == socket)
      task->resetSocket();
  }

  socket->deleteLater();
}


void Worker::remove()
{
  m_remove.clear();
}


void Worker::syncRequest()
{
  Socket *socket = qobject_cast<Socket*>(sender());
  if (!socket)
    return;

  if (socket->role() != Socket::ReceiverMaster) {
    socket->reject();
    return;
  }

  socket->acceptSync();
}


void Worker::incomingConnection(int socketDescriptor)
{
  Socket *socket = new Socket(this);
  if (!socket->setSocketDescriptor(socketDescriptor)) {
    socket->deleteLater();
    return;
  }

  SCHAT_DEBUG_STREAM("[SendFile] Worker::incomingConnection()" << "socket:" << socket);

  connect(socket, SIGNAL(released()), SLOT(released()));
  connect(socket, SIGNAL(syncRequest()), SLOT(syncRequest()));
  connect(socket, SIGNAL(handshake(QByteArray,char)), SLOT(handshake(QByteArray,char)));
}


void Worker::timerEvent(QTimerEvent *event)
{
  if (event->timerId() != m_timer->timerId()) {
    QTcpServer::timerEvent(event);
    return;
  }

  start();
}


void Worker::start()
{
  SCHAT_DEBUG_STREAM("[SendFile] Worker::start()")
  if (m_timer->isActive())
    m_timer->stop();

  if (isListening())
    return;

  if (!listen(QHostAddress::Any, m_port))
    m_timer->start(5000, this);
}


void Worker::updateTask(const QByteArray &id, const QVariantMap &data)
{
  SCHAT_DEBUG_STREAM("[SendFile] Worker::updateTask()" << SimpleID::encode(id))

  SendFileTask task = m_tasks.value(id);
  if (!task)
    return;

  Transaction *transaction = task->transaction();
  if (transaction->role() == SenderRole && !transaction->remote().isValid()) {
    Hosts hosts(data.value(LS("remote")).toList());
    if (hosts.isValid()) {
      transaction->setRemote(hosts);
      task->discovery();
    }
  }
}

} // namespace SendFile
