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

#include "ScaleRunnable.h"

ScaleRunnable::ScaleRunnable(const QImage &image, const QSize &size)
  : QObject()
  , m_image(image)
  , m_size(size)
{
}


void ScaleRunnable::run()
{
  emit finished(m_image.scaled(m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
