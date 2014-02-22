/* $Id: Idle.h 2596 2012-05-01 23:38:08Z IMPOMEZIA $
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

#ifndef _IDLE_H_
#define _IDLE_H_

#include <QObject>
#include <QCursor>
#include <QDateTime>
#include <QTimer>

class IdlePlatform;

/*!
 * Определяет время неактивности, для установки автоматического статуса Away.
 */
class Idle: public QObject
{
  Q_OBJECT

public:
  Idle(QObject *parent = 0);
  ~Idle();
  inline bool isActive() const      { return m_timer.isActive(); }
  inline bool usingPlatform() const { return m_platform; }

signals:
  void secondsIdle(int);

public slots:
  void start();
  void stop();

private slots:
  void doCheck();

private:
  int secondsIdle();

  IdlePlatform *m_platform; ///< Платформозависимая реализация.
  int m_idleTime;           ///< Время простоя.
  QDateTime m_since;        ///< Время последней активности.
  QDateTime m_startTime;    ///< Время запуска.
  QPoint m_lastPos;         ///< Последняя позиция мыши.
  QTimer m_timer;           ///< Таймер для проверки неактивности.
};


/*!
 * Базовый интерфейс для платформозависимого определения времени неактивности.
 */
class IdlePlatform
{
public:
  IdlePlatform();
  ~IdlePlatform();

  bool init();
  int secondsIdle();

private:
  class Private;
  Private *d;
};

#endif
