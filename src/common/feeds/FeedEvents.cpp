/* $Id: FeedEvents.cpp 3635 2013-04-12 15:40:07Z IMPOMEZIA $
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

#include "feeds/FeedEvents.h"

FeedEvents *FeedEvents::m_self = 0;

FeedEvents::FeedEvents(QObject *parent)
  : QObject(parent)
{
  m_self = this;
}


FeedEvents::~FeedEvents()
{
  qDeleteAll(m_queue);
  m_self = 0;
}


void FeedEvents::start(FeedEvent *event)
{
  if (m_self->m_queue.contains(event))
    return;

  m_self->m_queue.enqueue(event);
  if (m_self->m_queue.size() == 1)
    QTimer::singleShot(0, m_self, SLOT(start()));
}


void FeedEvents::start()
{
  while (!m_queue.isEmpty()) {
    FeedEvent *event = m_queue.dequeue();
    emit notify(*event);
    delete event;
  }
}
