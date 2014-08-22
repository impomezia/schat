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

#ifndef IMAGEITEM_H_
#define IMAGEITEM_H_

#include <QImage>

#include "data/UploadItem.h"

class ImageItem : public UploadItem
{
public:
  enum { Type = 0x51494D47 /*QIMG*/ };

  ImageItem();
  ImageItem(const QImage &image, int mode);
  bool isNull() const override;
  int type() const override { return Type; }
  QString toString() const override;
  static QString toSaveAs(const QString &path, const QString &ext, qint64 date);

  QByteArray raw; ///< Изображение пригодное для сохранения в файл или передачу по сети.
  QImage image;   ///< Изображение.
  QString saveAs; ///< Имя файла для сохрания изображения на диск.
};

#endif /* IMAGEITEM_H_ */
