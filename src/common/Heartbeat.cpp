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

#include <QBasicTimer>
#include <QTimerEvent>

#include <QDebug>

#include "Heartbeat.h"
#include "interfaces/IClient.h"
#include "SJMPPacket.h"

Heartbeat::Heartbeat(QObject *parent)
  : QObject(parent)
  , m_client(0)
  , m_timerState(Idle)
{
  m_timer = new QBasicTimer();
}


Heartbeat::~Heartbeat()
{
  delete m_timer;
}


void Heartbeat::onAuthRequired(IClient *client)
{
  Q_UNUSED(client)

  setTimerState(WaitAuth);
}


void Heartbeat::onConnectToHost(IClient *client, const QString &hostName, quint16 port)
{
  Q_UNUSED(hostName)
  Q_UNUSED(port)

  m_client = client;
  setTimerState(WaitConnect);
}


void Heartbeat::onDisconnected(IClient *client, int status)
{
  Q_UNUSED(client)
  Q_UNUSED(status)

  m_timer->stop();

  if (status != 1000)
    setTimerState(Reconnect);
}


void Heartbeat::onPacket(IClient *client, const SJMPPacket &packet)
{
  Q_UNUSED(client)
  Q_UNUSED(packet)

  if (packet.type() == SJMPPacket::Reply)
    setTimerState(Idle);
}


void Heartbeat::onPong(IClient *client)
{
  Q_UNUSED(client)

  setTimerState(Idle);
}


void Heartbeat::timerEvent(QTimerEvent *event)
{
  if (m_client && event->timerId() == m_timer->timerId()) {
    if (m_timerState == WaitConnect || m_timerState == WaitAuth || m_timerState == WaitPong) {
      m_client->leave();
    }
    else if (m_timerState == Idle) {
      setTimerState(WaitPong);
      m_client->ping();
    }
    else if (m_timerState == Reconnect) {
      m_client->reconnect();
    }
  }

  QObject::timerEvent(event);
}


void Heartbeat::setTimerState(TimerState state)
{
  m_timerState = state;
  if (m_timer->isActive())
    m_timer->stop();

  switch (state) {
    case WaitConnect:
      m_timer->start(20000, this);
      break;

    case WaitAuth:
      m_timer->start(5000, this);
      break;

    case Idle:
      m_timer->start(20000, this);
      break;

    case WaitPong:
      m_timer->start(15000, this);
      break;

    case Reconnect:
      m_timer->start(2000, this);
      break;
  }
}
