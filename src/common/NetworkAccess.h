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

#ifndef NETWORKACCESS_H_
#define NETWORKACCESS_H_

#include <QObject>

#include "interfaces/IDownloadItem.h"
#include "interfaces/INetworkListener.h"
#include "schat.h"

class INetworkHandler;

/*!
 * Высокоуровневый модуль доступа к сети.
 */
class SCHAT_EXPORT NetworkAccess : public QObject, public INetworkListener
{
  Q_OBJECT

public:
  NetworkAccess(QObject *parent = 0);
  bool canDownload(const QUrl &url) const;
  DownloadItem download(const QUrl &url, const QString &fileName = QString());
  void addHandler(INetworkHandler *handler);

signals:
  void handlerAdded();

private:
  qint64 m_counter;
  QList<INetworkHandler*> m_handlers;
  QMap<qint64, DownloadItem> m_items;
};

#endif // NETWORKACCESS_H_
