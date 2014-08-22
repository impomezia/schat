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

#include <QNetworkReply>

#include "uploaders/Uploader.h"
#include "uploaders/UploadResult.h"

Uploader::Uploader(QObject *parent)
  : QObject(parent)
{
}


void Uploader::prepare(QNetworkReply *reply, const ChatId &id)
{
  if (!reply)
    return;

  reply->setProperty("id", id.toByteArray());
  reply->setProperty("%",  0);

  connect(reply, SIGNAL(finished()), SLOT(onFinished()));
  connect(reply, SIGNAL(uploadProgress(qint64,qint64)), SLOT(onUploadProgress(qint64,qint64)));
}


void Uploader::onFinished()
{
  QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
  if (!reply)
    return;

  UploadResult result(reply);
  read(result, reply);

  emit finished(result);
}


void Uploader::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
  QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
  if (!reply)
    return;

  const int percent = bytesTotal == 0 ? 0 : bytesSent * 100 / bytesTotal;
  if (reply->property("%").toInt() != percent) {
    reply->setProperty("%", percent);
    emit uploadProgress(reply->property("id").toByteArray(), percent);
  }
}
