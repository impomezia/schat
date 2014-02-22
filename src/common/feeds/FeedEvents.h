/* $Id: FeedEvents.h 3635 2013-04-12 15:40:07Z IMPOMEZIA $
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

#ifndef FEEDEVENTS_H_
#define FEEDEVENTS_H_

#include <QObject>
#include <QQueue>

#include "feeds/FeedEvent.h"

class SCHAT_EXPORT FeedEvents : public QObject
{
  Q_OBJECT

public:
  FeedEvents(QObject *parent = 0);
  ~FeedEvents();
  inline static FeedEvents *i() { return m_self; }
  static void start(FeedEvent *event);

signals:
  void notify(const FeedEvent &event);

private slots:
  void start();

private:
  QQueue<FeedEvent*> m_queue; ///< Очередь отправки.
  static FeedEvents *m_self;  ///< Указатель на себя.
};

#endif /* FEEDEVENTS_H_ */
