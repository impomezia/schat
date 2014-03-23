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

#ifndef ISERVICETASK_H_
#define ISERVICETASK_H_

#include <QtGlobal>

class QObject;
class QNetworkAccessManager;

class IServiceTask
{
public:
  inline IServiceTask() : m_counter(0)   {}
  inline virtual ~IServiceTask()         {}
  inline qint64 counter() const          { return m_counter; }
  inline void setCounter(qint64 counter) { m_counter = counter; }
  virtual QObject *create(QNetworkAccessManager *net, QObject *parent) = 0;

private:
  qint64 m_counter;
};

#endif // ISERVICETASK_H_
