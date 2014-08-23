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

#include <QBuffer>
#include <QFile>
#include <QDir>

#include "data/ImageCrop.h"
#include "tasks/SaveTask.h"

SaveTask::SaveTask(const ChatId &id, const QImage &image, const QString &fileName, const QByteArray &raw, const QSize &resize)
  : QObject()
  , QRunnable()
  , m_id(id)
  , m_body(raw)
  , m_image(image)
  , m_size(resize)
  , m_fileName(fileName)
{
}


void SaveTask::run()
{
  if (!m_image.isNull()) {
    QBuffer buffer;

    if (m_body.isEmpty()) {
      buffer.setBuffer(&m_body);
      m_image.save(&buffer, "PNG");
    }

    Thumbnail thumb;
    if (m_size.isValid()) {
      thumb.image = ImageCrop::crop(m_image, m_size);

      buffer.close();
      buffer.setBuffer(&thumb.raw);
      thumb.image.save(&buffer, "PNG");
    }

    emit ready(m_id, m_body, thumb);
  }

  if (!m_fileName.isEmpty() && !m_body.isEmpty()) {
    QDir().mkpath(QFileInfo(m_fileName).absolutePath());

    QFile file(m_fileName);
    if (file.open(QFile::WriteOnly))
      file.write(m_body);
  }
}
