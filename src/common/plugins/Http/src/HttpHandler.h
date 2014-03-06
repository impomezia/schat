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

#ifndef HTTPHANDLER_H_
#define HTTPHANDLER_H_

#include <QObject>

#include "interfaces/INetworkHandler.h"

class HttpTask;

class HttpHandler : public QObject, public INetworkHandler
{
  Q_OBJECT

public:
  HttpHandler(HttpTask *task, QObject *parent = 0);
  void addListener(INetworkListener *listener) Q_DECL_OVERRIDE;
  void removeListener(INetworkListener *listener) Q_DECL_OVERRIDE;

private:
  HttpTask *m_task;
  QList<INetworkListener*> m_listeners;
};

#endif // HTTPHANDLER_H_
