/* $Id: NodeWorker.cpp 3762 2013-08-03 03:08:19Z IMPOMEZIA $
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

#include <QCoreApplication>
#include <QReadLocker>

#include "events.h"
#include "net/NodeWorker.h"
#include "net/SimpleSocket.h"
#include "NodeLog.h"

#define LOG_N4010  LOG_INFO("N4010", "Core/Worker", "s:" << socket->id() << ". new connection: ip:" << socket->peerAddress().toString() << ":" << socket->peerPort())
#define LOG_N4011  LOG_INFO("N4011", "Core/Worker", "s:" << socket->id() << ". released: ip:" << socket->peerAddress().toString() << ":" << socket->peerPort() << ", authorized:" << socket->isAuthorized())
#define LOG_N4012 LOG_DEBUG("N4012", "Core/Worker", "released:" << id)

NodeWorker::NodeWorker(QObject *core)
  : QThread()
  , m_core(core)
{
}


void NodeWorker::run()
{
  NodeWorkerListener listener(m_core);

  emit ready(&listener);

  exec();
}


NodeWorkerListener::NodeWorkerListener(QObject *core)
  : QObject()
  , m_core(core)
{
}


NodeWorkerListener::~NodeWorkerListener()
{
  foreach (SimpleSocket *socket, m_sockets) {
    socket->abort();
    socket->deleteLater();
  }
}


int NodeWorkerListener::count()
{
  QReadLocker lock(&m_lock);
  return m_sockets.size();
}


void NodeWorkerListener::customEvent(QEvent *event)
{
  int type = event->type();
  if (type == ServerEvent::NewConnection)
    add(static_cast<NewConnectionEvent*>(event));
  else if (type == ServerEvent::NewPackets)
    packets(static_cast<NewPacketsEvent*>(event));
}


/*!
 * Обработка новых пакетов полученных от сокета.
 *
 * \param id      Идентификатор сокета.
 * \param packets Тела пакетов.
 */
void NodeWorkerListener::packets(quint64 id, const QList<QByteArray> &packets)
{
  m_lock.lockForRead();
  SimpleSocket *socket = m_sockets.value(id);
  m_lock.unlock();

  if (!socket)
    return;

  NewPacketsEvent *event = new NewPacketsEvent(QList<quint64>() << id, packets, socket->channelId());
  if (!socket->isAuthorized())
    event->address = socket->peerAddress();

  QCoreApplication::postEvent(m_core, event);
}


void NodeWorkerListener::released(quint64 id)
{
  m_lock.lockForRead();
  SimpleSocket *socket = m_sockets.value(id);
  m_lock.unlock();
  LOG_N4012

  if (socket) {
    LOG_N4011

    if (socket->isAuthorized()) {
      SocketReleaseEvent *event = new SocketReleaseEvent(id, socket->errorString(), socket->channelId());
      QCoreApplication::postEvent(m_core, event);
    }

    m_lock.lockForWrite();
    m_sockets.remove(id);
    socket->deleteLater();
    m_lock.unlock();
  }
}


/*!
 * Добавление нового подключения.
 */
void NodeWorkerListener::add(NewConnectionEvent *event)
{
  SimpleSocket *socket = new SimpleSocket();
  socket->setId(event->socket());

  if (socket->setSocketDescriptor(event->socketDescriptor)) {
    LOG_N4010

    connect(socket, SIGNAL(newPackets(quint64,QList<QByteArray>)), SLOT(packets(quint64,QList<QByteArray>)), Qt::DirectConnection);
    connect(socket, SIGNAL(released(quint64)), SLOT(released(quint64)), Qt::DirectConnection);

    m_lock.lockForWrite();
    m_sockets[socket->id()] = socket;
    m_lock.unlock();
  }
  else
    socket->deleteLater();
}


void NodeWorkerListener::packets(NewPacketsEvent *event)
{
  QList<SimpleSocket*> sockets;

  m_lock.lockForRead();
  foreach (quint64 id, event->sockets()) {
    if (m_sockets.contains(id)) {
      SimpleSocket *socket = m_sockets.value(id);
      if (socket)
        sockets.append(socket);
    }
  }
  m_lock.unlock();

  for (int i = 0; i < sockets.size(); ++i) {
    SimpleSocket *socket = sockets.at(i);

    socket->setDate(event->timestamp);
    socket->send(event->packets);

    if (!event->option)
      continue;

    if (event->option == NewPacketsEvent::KillSocketOption)
      socket->leave();
    else if (event->option == NewPacketsEvent::AuthorizeSocketOption && !socket->isAuthorized())
      socket->setAuthorized(event->channelId());
  }
}
