/* $Id: SendFileTask.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QBasicTimer>
#include <QFile>
#include <QTimerEvent>

#include "DateTime.h"
#include "debugstream.h"
#include "net/SimpleID.h"
#include "SendFileSocket.h"
#include "SendFileTask.h"
#include "SendFileTransaction.h"
#include "SendFileWorker.h"

namespace SendFile {

Task::Task(const QVariantMap &data)
  : QObject(0)
  , m_finished(false)
  , m_file(0)
  , m_pos(0)
  , m_time(0)
  , m_socket(0)
{
  SCHAT_DEBUG_STREAM("[SendFile] Task::Task(), id:" << SimpleID::encode(data.value("id").toByteArray()) << this);

  m_timer = new QBasicTimer();
  m_transaction = new Transaction(data);
  m_file = new QFile(m_transaction->file().name, this);
}


Task::~Task()
{
  SCHAT_DEBUG_STREAM("[SendFile] Task::~Task()" << this);

  if (m_timer->isActive())
    m_timer->stop();

  delete m_timer;
  delete m_transaction;
}


/*!
 * Обработка приветствия от входящего подключения.
 *
 * \param socket Указатель на сокет.
 * \param role   Роль удалённой стороны.
 *
 * \return \b false если произошла ошибка и сокет должен быть отвергнут.
 */
bool Task::handshake(Socket *socket, char role)
{
  SCHAT_DEBUG_STREAM("[SendFile] Task::handshake(), socket:" << socket << "role:" << role << this);

  if (m_socket)
    return false;

  // Удалённая сторона получатель файла.
  if (role == Socket::ReceiverSlave) {
    if (m_transaction->role())
      return false;

    socket->setRole(Socket::SenderMaster);
    socket->sync();
    m_socket = socket;
    start();
    return true;
  }
  else if (role == Socket::SenderSlave) {
    if (!m_transaction->role())
      return false;

    socket->setRole(Socket::ReceiverMaster);
    socket->accept();
    m_socket = socket;
    start();
    return true;
  }

  return false;
}


bool Task::init()
{
  if (!m_file->open(m_transaction->role() ? QIODevice::WriteOnly : QIODevice::ReadOnly))
    return false;

  discovery();
  return true;
}


bool Task::isReadyToRemove() const
{
  if (m_finished && !m_timer->isActive())
    return true;

  return false;
}


/*!
 * Попытка подключения к удалённой стороне.
 */
void Task::discovery()
{
  if (m_finished || m_socket)
    return;

  const Hosts &remote = m_transaction->remote();
  if (!remote.isValid())
    return;

  QString host = remote.address(Internal);
  quint16 port = remote.port(Internal);
  discovery(host, port);

  if (remote.address() != host || remote.port() != port)
    discovery(remote.address(), remote.port());
}



void Task::stop()
{
  m_finished = true;
  QList<Socket *> sockets = findChildren<Socket *>();
  foreach (Socket *socket, sockets) {
    socket->leave();
    socket->deleteLater();
  }

  m_socket = 0;
  m_discovery.clear();
  m_timer->start(10000, this);
}


void Task::timerEvent(QTimerEvent *event)
{
  if (event->timerId() != m_timer->timerId()) {
    QObject::timerEvent(event);
    return;
  }

  if (!m_finished) {
    qint64 total = m_transaction->file().size;
    emit progress(m_transaction->id(), m_pos, total, m_pos * 100 / total);
  }
  else {
    m_timer->stop();
    emit released(m_transaction->id());
  }
}


void Task::accepted()
{
  Socket *socket = qobject_cast<Socket*>(sender());
  if (!socket)
    return;

  SCHAT_DEBUG_STREAM("[SendFile] Task::accepted(), socket:" << socket << this);

  if (!m_socket) {
    m_socket = socket;
    start();
    m_discovery.removeAll(socket);
  }

  foreach (Socket *socket, m_discovery) {
    socket->leave();
  }
}



void Task::acceptRequest()
{
  Socket *socket = qobject_cast<Socket*>(sender());
  if (!socket || socket->role() != Socket::SenderSlave)
    return;

  SCHAT_DEBUG_STREAM("[SendFile] Task::acceptRequest(), socket:" << socket << this);

  if (m_socket)
    m_socket->leave();

  m_socket = socket;
  m_discovery.removeAll(socket);

  start();
  m_socket->sync();
  stopDiscovery();
}


void Task::finished()
{
  m_timer->stop();
  if (m_socket)
    m_socket->leave();

  emit finished(m_transaction->id(), DateTime::utc() - m_time);
  m_finished = true;

  if (!m_discovery.isEmpty())
    stopDiscovery();
  else
    emit released(m_transaction->id());
}


void Task::progress(qint64 current)
{
  if (!m_timer->isActive()) {
    m_time = DateTime::utc();
    m_timer->start(200, this);
    emit started(m_transaction->id(), m_time);
  }

  m_pos = current;
}


void Task::released()
{
  Socket *socket = qobject_cast<Socket*>(sender());
  if (!socket)
    return;

  SCHAT_DEBUG_STREAM("[SendFile] Task::released(), socket:" << socket << this);

  if (m_finished || m_socket || socket->reconnect())
    m_discovery.removeAll(socket);

  if (m_finished && m_discovery.isEmpty())
    emit released(m_transaction->id());
}


/*!
 * Обработка запроса на синхронизацию, если отправителем файла является сервер.
 */
void Task::syncRequest()
{
  Socket *socket = qobject_cast<Socket*>(sender());
  if (!socket)
    return;

  SCHAT_DEBUG_STREAM("[SendFile] Task::syncRequest(), socket:" << socket << this);

  if (socket->role() != Socket::ReceiverSlave || m_socket) {
    socket->reject();
    return;
  }

  m_socket = socket;
  m_discovery.removeAll(socket);

  start();
  m_socket->acceptSync();
  stopDiscovery();
}


/*!
 * Попытка подключения к удалённой стороне.
 *
 * \param host Адрес удалённой стороны.
 * \param port Порт удалённой стороны.
 */
void Task::discovery(const QString &host, quint16 port)
{
  SCHAT_DEBUG_STREAM("[SendFile] Task::discovery(), host:" << host << ", port:" << port << this);

  Socket *socket = new Socket(host, port, m_transaction->id(), m_transaction->role(), this);
  connect(socket, SIGNAL(released()), SLOT(released()));
  connect(socket, SIGNAL(acceptRequest()), SLOT(acceptRequest()));
  connect(socket, SIGNAL(syncRequest()), SLOT(syncRequest()));
  m_discovery.append(socket);
}


void Task::start()
{
  SCHAT_DEBUG_STREAM("[SendFile] Task::start(), socket:" << m_socket << this);

  connect(m_socket, SIGNAL(progress(qint64,qint64)), SLOT(progress(qint64)));
  connect(m_socket, SIGNAL(finished()), SLOT(finished()));
  m_socket->setFile(m_file, m_transaction->file().size);
}



void Task::stopDiscovery()
{
  foreach (Socket *socket, m_discovery) {
    socket->leave();
  }
}

} // namespace SendFile
