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

#include <QDir>
#include <QImageReader>
#include <QTime>

#include "Path.h"
#include "PreviewDB.h"
#include "PreviewItem.h"
#include "PreviewRunnable.h"
#include "sglobal.h"

PreviewRunnable::PreviewRunnable(const QString &id)
  : QObject()
  , QRunnable()
  , m_id(id)
  , m_path(PreviewItem::path())
  , m_source(QDir::tempPath() + LC('/') + id)
{
}


void PreviewRunnable::run()
{
  SCHAT_DEBUG_CODE(QTime t; t.start();)

  ImageRecord record;
  record.id = m_id;

  QFile file(m_source);
  QImageReader reader;
  reader.setDevice(&file);

  if (!reader.canRead()) {
    file.remove();
    emit finished(record);
    return;
  }

  const QString format = reader.format();
  if (reader.loopCount() != 0)
    record.flags |= PreviewItem::Animated;

  const QImage image   = reader.read();
  file.rename(prepare(LS("o"), format));

  QImage thumb;
  if (image.width() > 90 && image.height() > 90)
    thumb = image.scaled(90, 90, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
  else
    thumb = image;

  if (thumb.height() > 90) {
    thumb = thumb.copy(0, (thumb.height() - 90) / 2, 90, 90);
  }
  else if (thumb.width() > 90) {
    thumb = thumb.copy((thumb.width() - 90) / 2, 0, 90, 90);
  }

  thumb.save(prepare(LS("t"), LS("png")), "PNG");

  record.width  = image.width();
  record.height = image.height();
  record.format = format;

  emit finished(record);

  SLOG_DEBUG(t.elapsed() << "ms" << image.size());
}


QString PreviewRunnable::prepare(const QString &type, const QString &format) const
{
  const QString path = QString(LS("%1/%2/%3")).arg(m_path, type, m_id.left(2));
  QDir().mkpath(path);

  const QString dest = QString(LS("%1/%2.%3")).arg(path, m_id, format);

  if (QFile::exists(dest))
    QFile::remove(dest);

  return dest;
}
