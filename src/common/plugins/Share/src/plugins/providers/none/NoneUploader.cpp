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

#include "data/ImageItem.h"
#include "sglobal.h"
#include "NoneUploader.h"
#include "uploaders/UploadResult.h"

NoneUploader::NoneUploader(QObject *parent)
  : Uploader(parent)
{
}


void NoneUploader::upload(QNetworkAccessManager *net, UploadItemPtr item, const QVariant &data)
{
  Q_UNUSED(net)
  Q_UNUSED(data)

  UploadResult result;
  result.id       = item->id();
  result.status   = 200;
  result.provider = LS("none");

  if (item->type() == ImageItem::Type) {
    ImageItem *i = uploaditem_cast<ImageItem*>(item.data());
    UploadResult::Image image;
    image.link   = QUrl::fromLocalFile(i->saveAs);
    image.height = i->image.height();
    image.size   = i->raw.size();
    image.width  = i->image.width();
    result.images.append(image);
  }

  emit finished(result);
}


void NoneUploader::read(UploadResult &result, QNetworkReply *reply)
{
  Q_UNUSED(result)
  Q_UNUSED(reply)
}
