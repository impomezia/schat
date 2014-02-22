/* $Id: Idle.cpp 2595 2012-05-01 21:45:36Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
 * Copyright © 2003 Justin Karneges <justin@affinix.com> (from KVIrc source code)
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

#include <QCursor>
#include <QDateTime>
#include <QTimer>

#include "Idle.h"

Idle::Idle(QObject *parent)
  : QObject(parent)
  , m_platform(0)
  , m_idleTime(0)
{
  // try to use platform idle
  m_platform = new IdlePlatform();
  if (!m_platform->init()) {
    delete m_platform;
    m_platform = 0;
  }

  connect(&m_timer, SIGNAL(timeout()), SLOT(doCheck()));
}

Idle::~Idle()
{
  if (m_platform)
    delete m_platform;
}


void Idle::start()
{
  m_startTime = QDateTime::currentDateTime();

  if (!m_platform) {
    // generic idle
    m_lastPos = QCursor::pos();
    m_since = QDateTime::currentDateTime();
  }

  // poll every second (use a lower value if you need more accuracy)
  m_timer.start(1000);
}


void Idle::stop()
{
  m_timer.stop();
}


int Idle::secondsIdle()
{
  int i;
  if (m_platform)
    i = m_platform->secondsIdle();
  else {
    QPoint mousePos = QCursor::pos();
    QDateTime dateTime = QDateTime::currentDateTime();
    if (m_lastPos != mousePos) {
      m_lastPos = mousePos;
      m_since = dateTime;
    }
    i = m_since.secsTo(dateTime);
  }

  // set 'beginIdle' to the beginning of the idle time (by backtracking 'i' seconds from now)
  QDateTime beginIdle = QDateTime::currentDateTime().addSecs(-i);

  // set 't' to hold the number of seconds between 'beginIdle' and 'startTime'
  int t = beginIdle.secsTo(m_startTime);

  // beginIdle later than (or equal to) startTime?
  if (t <= 0) {
    // scoot ourselves up to the new idle start
    m_startTime = beginIdle;
  }
  // beginIdle earlier than startTime?
  else if (t > 0) {
    // do nothing
  }

  // how long have we been idle?
  m_idleTime = m_startTime.secsTo(QDateTime::currentDateTime());
  return m_idleTime;
}


void Idle::doCheck()
{
  emit secondsIdle(secondsIdle());
}
