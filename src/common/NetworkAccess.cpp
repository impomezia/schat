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

#include "DateTime.h"
#include "interfaces/INetworkError.h"
#include "interfaces/INetworkHandler.h"
#include "NetworkAccess.h"

NetworkAccess::NetworkAccess(QObject *parent)
  : QObject(parent)
{
}


bool NetworkAccess::canDownload(const QUrl &url) const
{
  SLOG_DEBUG(url);

  foreach (INetworkHandler *handler, m_handlers) {
    if (handler->canDownload(url))
      return true;
  }

  return false;
}


DownloadItem NetworkAccess::download(const QUrl &url, const QString &fileName, const QVariantMap &options)
{
  SLOG_DEBUG(url << fileName);

  DownloadItem item;

  if (!canDownload(url))
    return item;

  foreach (INetworkHandler *handler, m_handlers) {
    item = handler->download(url, fileName, options);
    if (item)
      break;
  }

  if (!item)
    return item;

  Q_ASSERT(item->url() == url);

  item->setStartDate(DateTime::utc());

  m_items.insert(url, item);
  return item;
}


void NetworkAccess::addHandler(INetworkHandler *handler)
{
  handler->addListener(this);
  m_handlers.append(handler);

  emit handlerAdded();
}


void NetworkAccess::onDownloadProgress(const QUrl &url, qint64 bytesReceived, qint64 bytesTotal)
{
  DownloadItem item = m_items.value(url);
  if (!item)
    return;

  emit downloadProgress(item, bytesReceived, bytesTotal);
}


void NetworkAccess::onFinished(const QUrl &url, INetworkError *error)
{
  SCHAT_DEBUG_CODE(if (error) SLOG_DEBUG(url << error->error() << error->status()); else SLOG_DEBUG(url));

  DownloadItem item = m_items.value(url);
  if (!item)
    return;

  item->setEndDate(DateTime::utc());
  item->setError(error);

  emit finished(item);
}


void NetworkAccess::onReadyRead(const QUrl &url, const QByteArray &data)
{
  DownloadItem item = m_items.value(url);
  if (!item)
    return;

  item->data().append(data);
}
