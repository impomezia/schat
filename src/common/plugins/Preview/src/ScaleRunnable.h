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

#ifndef SCALERUNNABLE_H_
#define SCALERUNNABLE_H_

#include <QObject>
#include <QImage>
#include <QRunnable>

class ScaleRunnable : public QObject, public QRunnable
{
  Q_OBJECT

public:
  ScaleRunnable(const QImage &image, const QSize &size);
  void run() Q_DECL_OVERRIDE;

signals:
  void finished(const QImage &image);

private:
  const QImage m_image;
  const QSize m_size;
};

#endif // SCALERUNNABLE_H_
