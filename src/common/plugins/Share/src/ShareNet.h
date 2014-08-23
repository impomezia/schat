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

#ifndef SHARENET_H_
#define SHARENET_H_

#include <QMap>
#include <QObject>

#include "data/UploadItem.h"
#include "interfaces/IServiceTask.h"

class AbstractUploader;
class QNetworkAccessManager;
class Uploader;
class Providers;
class UploadResult;

class ShareNet : public QObject
{
  Q_OBJECT

public:
  ShareNet(Providers *providers, QNetworkAccessManager *net, QObject *parent = 0);
  ~ShareNet();
  inline QNetworkAccessManager *net() const { return m_net; }

signals:
  void finished(const UploadResult &result);
  void uploadProgress(const ChatId &id, int percent);

public slots:
  void add(UploadItemPtr item, const QString &provider, const QVariant &data);

private:
  QMap<QString, Uploader*> m_uploaders; ///< Список загрузчиков файла.
  QNetworkAccessManager *m_net;         ///< Менеджер доступа к сети.
};


class ShareNetTask : public IServiceTask
{
public:
  ShareNetTask(Providers *providers) : m_providers(providers) {}
  QObject *create(QNetworkAccessManager *net, QObject *parent);

private:
  Providers *m_providers;
};

#endif /* SHARENET_H_ */
