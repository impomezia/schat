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

#include <QSslSocket>
#include <QUrl>

#include "HttpDownloadItem.h"
#include "HttpHandler.h"
#include "HttpTask.h"
#include "interfaces/INetworkListener.h"
#include "sglobal.h"

HttpHandler::HttpHandler(HttpTask *task, QObject *parent)
  : QObject(parent)
  , m_task(task)
{
  connect(task, SIGNAL(readyRead(QUrl,QByteArray)), SLOT(onReadyRead(QUrl,QByteArray)));
  connect(task, SIGNAL(downloadProgress(QUrl,qint64,qint64)), SLOT(onDownloadProgress(QUrl,qint64,qint64)));
  connect(task, SIGNAL(finished(QUrl,INetworkError*)), SLOT(onFinished(QUrl,INetworkError*)));
}


bool HttpHandler::canDownload(const QUrl &url) const
{
  if (url.scheme() == LS("http"))
    return true;

  if (url.scheme() == LS("https") && QSslSocket::supportsSsl())
    return true;

  return false;
}


DownloadItem HttpHandler::download(const QUrl &url, const QString &fileName, const QVariantMap &options)
{
  HttpDownloadItem *item = new HttpDownloadItem(url, fileName);

  QMetaObject::invokeMethod(m_task, "download", Qt::QueuedConnection, Q_ARG(QUrl, url), Q_ARG(QString, fileName), Q_ARG(QVariantMap, options));

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


void HttpHandler::onDownloadProgress(const QUrl &url, qint64 bytesReceived, qint64 bytesTotal)
{
  foreach (INetworkListener *listener, m_listeners)
    listener->onDownloadProgress(url, bytesReceived, bytesTotal);
}


void HttpHandler::onFinished(const QUrl &url, INetworkError *error)
{
  foreach (INetworkListener *listener, m_listeners)
    listener->onFinished(url, error);
}


void HttpHandler::onReadyRead(const QUrl &url, const QByteArray &data)
{
  foreach (INetworkListener *listener, m_listeners)
    listener->onReadyRead(url, data);
}
