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
#include "DateTime.h"
#include "sglobal.h"

ImageItem::ImageItem()
  : UploadItem(-1)
{
  init();
}


ImageItem::ImageItem(const QImage &image, int mode)
  : UploadItem(mode)
  , image(image)
{
  init();
}


bool ImageItem::isNull() const
{
  return UploadItem::isNull() || image.isNull();
}


QString ImageItem::toString() const
{
  return QString(LS("%1 %2x%3 - %4")).arg(UploadItem::toString()).arg(image.width()).arg(image.height()).arg(raw.size());
}


QString ImageItem::toSaveAs(const QString &path, const QString &ext, qint64 date)
{
  const QDateTime dt = DateTime::toDateTime(date);
  return path + dt.toString(LS("/yyyy-MM/")) + dt.toString(LS("yyyy-MM-dd_hh-mm-ss.")) + ext;
}
