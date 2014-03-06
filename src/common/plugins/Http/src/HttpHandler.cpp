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

#include <QUrl>
#include <QSslSocket>

#include "HttpDownloadItem.h"
#include "HttpHandler.h"
#include "sglobal.h"

HttpHandler::HttpHandler(HttpTask *task, QObject *parent)
  : QObject(parent)
  , m_task(task)
{
}


bool HttpHandler::canDownload(const QUrl &url) const
{
  if (url.scheme() == LS("http"))
    return true;

  if (url.scheme() == LS("https") && QSslSocket::supportsSsl())
    return true;

  return false;
}


DownloadItem HttpHandler::download(const qint64 &id, const QUrl &url, const QString &fileName)
{
  SLOG_DEBUG(id);

  HttpDownloadItem *item = new HttpDownloadItem(id, url, fileName);

  return DownloadItem(item);
}


void HttpHandler::addListener(INetworkListener *listener)
{
  Q_ASSERT(listener);

  if (!m_listeners.contains(listener))
    m_listeners.append(listener);
}


void HttpHandler::removeListener(INetworkListener *listener)
{
  m_listeners.removeAll(listener);
}
