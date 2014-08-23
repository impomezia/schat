/*   $Id$
 *   Copyright (C) 2013 Alexander Sedov <imp@schat.me>
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

#include "data/ImageCrop.h"

QImage ImageCrop::crop(const QImage &image, const QSize &size)
{
  if (image.width() > 32 || image.height() > 32)
    return image.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

  return image;
}
