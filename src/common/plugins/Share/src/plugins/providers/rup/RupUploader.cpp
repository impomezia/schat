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

#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "data/ImageItem.h"
#include "DateTime.h"
#include "JSON.h"
#include "sglobal.h"
#include "RupUploader.h"
#include "uploaders/UploadResult.h"

RupUploader::RupUploader(QObject *parent)
  : Uploader(parent)
{
}


void RupUploader::upload(QNetworkAccessManager *net, UploadItemPtr item, const QVariant &data)
{
  const QVariantMap map = data.toMap();

  QNetworkReply *reply = 0;

  if (item->type() == ImageItem::Type) {
    ImageItem *i = uploaditem_cast<ImageItem*>(item.data());

    QHttpPart image;
    image.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
    image.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"image\"; filename=\"" + DateTime::toDateTime(item->date()).toString(LS("yyyy-MM-dd_hh-mm-ss")).toLatin1() + ".png\"");
    image.setBody(i->raw);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->append(image);

    if (!item->title().isEmpty()) {
      QHttpPart title;
      title.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"title\""));
      title.setBody(item->title().toUtf8());
      multiPart->append(title);
    }

    QNetworkRequest request(QUrl(LS("https://api.schat.me/1/image")));

    if (map.contains(LS("token"))) {
      request.setRawHeader("Authorization", "Bearer " + map.value(LS("token")).toByteArray());
    }

    reply = net->post(request, multiPart);
    multiPart->setParent(reply);
  }

  prepare(reply, item->id());
}


void RupUploader::read(UploadResult &result, QNetworkReply *reply)
{
  if (result.status != 200)
    return;

  const QVariantMap json = JSON::parse(reply->readAll()).toMap().value(LS("data")).toList().value(0).toMap();

  UploadResult::Image image;
  image.id         = json.value(LS("id")).toString();
  image.link       = json.value(LS("link")).toUrl();
  image.deletehash = json.value(LS("deletehash")).toString();
  image.title      = json.value(LS("title")).toString();
  image.height     = json.value(LS("height")).toInt();
  image.size       = json.value(LS("size")).toInt();
  image.width      = json.value(LS("width")).toInt();

  result.provider  = LS("rup");
  result.json      = json;
  result.desc      = json.value(LS("description")).toString();
  result.images.append(image);
}
