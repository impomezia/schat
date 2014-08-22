/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#ifndef UPLOADER_H_
#define UPLOADER_H_

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class UploadResult;

#include "data/UploadItem.h"

class Uploader : public QObject
{
  Q_OBJECT

public:
  Uploader(QObject *parent = 0);
  virtual void upload(QNetworkAccessManager *net, UploadItemPtr item, const QVariant &data) = 0;

signals:
  void finished(const UploadResult &result);
  void uploadProgress(const ChatId &id, int percent);

protected:
  virtual void read(UploadResult &result, QNetworkReply *reply) = 0;
  void prepare(QNetworkReply *reply, const ChatId &id);

private slots:
  void onFinished();
  void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);
};

#endif /* UPLOADER_H_ */
