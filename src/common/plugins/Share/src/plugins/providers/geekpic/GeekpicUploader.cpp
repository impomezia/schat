/* Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

#include <QHttpMultiPart>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "data/ImageItem.h"
#include "DateTime.h"
#include "GeekpicUploader.h"
#include "JSON.h"
#include "sglobal.h"
#include "uploaders/UploadResult.h"

GeekpicUploader::GeekpicUploader(QObject *parent)
  : Uploader(parent)
{
}


void GeekpicUploader::upload(QNetworkAccessManager *net, UploadItemPtr item, const QVariant &data)
{
  Q_UNUSED(data)

  QNetworkReply *reply = 0;

  if (item->type() == ImageItem::Type) {
    ImageItem *i = uploaditem_cast<ImageItem*>(item.data());
    const QByteArray fileName = DateTime::toDateTime(item->date()).toString(LS("yyyy-MM-dd_hh-mm-ss")).toLatin1() + ".png";

    QHttpPart image;
    image.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"image\"");
    image.setBody(i->raw.toBase64());

    QHttpPart name;
    name.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"name\"");
    name.setBody(fileName);

    QHttpPart description;
    description.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"description\"");
    if (!item->title().isEmpty())
      description.setBody(item->title().toUtf8());
    else
      description.setBody(fileName);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->append(image);
    multiPart->append(name);
    multiPart->append(description);

    QNetworkRequest request(QUrl(LS("http://geekpic.net/client.php")));

    reply = net->post(request, multiPart);
    reply->setProperty("width",  i->image.width());
    reply->setProperty("height", i->image.height());
    reply->setProperty("size",   i->raw.size());
    multiPart->setParent(reply);
  }

  prepare(reply, item->id());
}


void GeekpicUploader::read(UploadResult &result, QNetworkReply *reply)
{
  if (result.status != 200)
    return;

  const QVariantMap json = JSON::parse(reply->readAll()).toMap();

  UploadResult::Image image;
  image.id         = json.value(LS("id")).toString();
  image.link       = QUrl(LS("http://geekpic.net/pm-") + image.id + LS(".html"));
  image.height     = reply->property("height").toInt();
  image.size       = reply->property("size").toInt();
  image.width      = reply->property("width").toInt();

  result.provider  = LS("pic4net");
  result.json      = json;
  result.images.append(image);
}
