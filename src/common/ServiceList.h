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

#ifndef SERVICELIST_H_
#define SERVICELIST_H_

#include <QObject>
#include <QQueue>

#include "schat.h"

class IServiceTask;
class QNetworkAccessManager;

/*!
 * Список сервисных задач.
 */
class ServiceList : public QObject
{
  Q_OBJECT

public:
  ServiceList();
  inline int size() const { return m_tasks.size(); }

signals:
  void ready(qint64 counter, QObject *object);

public slots:
  void append(IServiceTask *task);
  void append(QQueue<IServiceTask*> queue);

private:
  QList<QObject*> m_tasks;      ///< Созданные и запущенные задачи.
  QNetworkAccessManager *m_net; ///< Менеджер доступа к сети.
};

#endif // SERVICELIST_H_
