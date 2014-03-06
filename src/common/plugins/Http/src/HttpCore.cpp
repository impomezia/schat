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

#include "ChatCore.h"
#include "HttpCmd.h"
#include "HttpCore.h"
#include "HttpHandler.h"
#include "HttpTask.h"
#include "NetworkAccess.h"
#include "ServiceThread.h"

HttpCore::HttpCore(QObject *parent)
  : ChatPlugin(parent)
{
  connect(ChatCore::service(), SIGNAL(ready(qint64,QObject*)), SLOT(onTaskReady(qint64,QObject*)));

  m_httpTaskId = ChatCore::service()->add(new HttpTaskCreator());
}


void HttpCore::chatReady()
{
  new HttpCmd(this);
}


void HttpCore::onTaskReady(qint64 counter, QObject *object)
{
  if (counter == m_httpTaskId) {
    HttpTask *task = qobject_cast<HttpTask*>(object);
    if (!task)
      return;

    ChatCore::networkAccess()->addHandler(new HttpHandler(task, this));
  }
}
