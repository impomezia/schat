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
#include "schat.h"

class HttpTask;

class HttpHandler : public QObject, public INetworkHandler
{
  Q_OBJECT

public:
  HttpHandler(HttpTask *task, QObject *parent = 0);
  bool canDownload(const QUrl &url) const Q_DECL_OVERRIDE;
  DownloadItem download(const QUrl &url, const QString &fileName = QString(), const QVariantMap &options = QVariantMap()) Q_DECL_OVERRIDE;
  void addListener(INetworkListener *listener) Q_DECL_OVERRIDE;
  void removeListener(INetworkListener *listener) Q_DECL_OVERRIDE;

private slots:
  void onDownloadProgress(const QUrl &url, qint64 bytesReceived, qint64 bytesTotal);
  void onFinished(const QUrl &url, INetworkError *error);
  void onReadyRead(const QUrl &url, const QByteArray &data);

private:
  HttpTask *m_task;
  QList<INetworkListener*> m_listeners;
};

#endif // HTTPHANDLER_H_
