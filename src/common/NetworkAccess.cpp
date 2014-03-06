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

#include "NetworkAccess.h"
#include "interfaces/INetworkHandler.h"

NetworkAccess::NetworkAccess(QObject *parent)
  : QObject(parent)
  , m_counter(0)
{
}


bool NetworkAccess::canDownload(const QUrl &url) const
{
  SLOG_DEBUG(url);

  return false;
}


DownloadItem NetworkAccess::download(const QUrl &url, const QString &fileName)
{
  SLOG_DEBUG(url << fileName);

  return DownloadItem();
}


void NetworkAccess::addHandler(INetworkHandler *handler)
{
  handler->addListener(this);
  m_handlers.append(handler);

  emit handlerAdded();
}
