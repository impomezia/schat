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

#ifndef HTTPTASK_H_
#define HTTPTASK_H_

#include <QObject>

#include "interfaces/IServiceTask.h"

class HttpTask : public QObject
{
  Q_OBJECT

public:
  HttpTask(QNetworkAccessManager *net, QObject *parent = 0);

private:
  QNetworkAccessManager *m_net;
};


class HttpTaskCreator : public IServiceTask
{
public:
  inline HttpTaskCreator() {}
  QObject *create(QNetworkAccessManager *net, QObject *parent) Q_DECL_OVERRIDE;
};

#endif // HTTPTASK_H_
