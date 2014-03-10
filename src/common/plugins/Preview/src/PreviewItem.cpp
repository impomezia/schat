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

#include "Path.h"
#include "PreviewDB.h"
#include "PreviewItem.h"
#include "sglobal.h"

QString PreviewItem::m_path;

PreviewItem::PreviewItem(const QUrl &url)
  : m_url(url)
  , m_flags(0)
  , m_height(0)
  , m_size(0)
  , m_width(0)
  , m_state(Downloading)
{
  m_id.init(url.toEncoded(), ChatId::NormalizedId);
}


QString PreviewItem::fileName(FileType type) const
{
  const QString id = m_id.toString();

  if (type == Original)
    return QString(LS("%1/o/%2/%3.%4")).arg(path(), id.left(2), id, m_format);

  if (type == Thumbnail)
    return QString(LS("%1/t/%2/%3.png")).arg(path(), id.left(2), id);

  return QString();
}


void PreviewItem::setDownloadItem(DownloadItem item)
{
  m_state = Downloading;
  m_item  = item;
}


void PreviewItem::setRecord(const ImageRecord &record)
{
  if (record.id.isNull())
    return setState(Downloading);

  if (record.width && record.height) {
    m_flags  = record.flags;
    m_height = record.height;
    m_size   = record.size;
    m_width  = record.width;
    m_format = record.format;

    return setState(Ready);
  }

  setState(Error);
}


QString PreviewItem::path()
{
  if (m_path.isEmpty())
    m_path = Path::cache() + LS("/images");

  return m_path;
}
