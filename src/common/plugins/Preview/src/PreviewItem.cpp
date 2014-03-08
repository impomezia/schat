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

#include "PreviewDB.h"
#include "PreviewItem.h"

PreviewItem::PreviewItem(const QUrl &url)
  : m_record(0)
  , m_url(url)
  , m_state(Downloading)
{
  m_id.init(url.toEncoded(), ChatId::NormalizedId);
}


PreviewItem::~PreviewItem()
{
  if (m_record)
    delete m_record;
}


void PreviewItem::setDownloadItem(DownloadItem item)
{
  m_state = Downloading;
  m_item  = item;
}


void PreviewItem::setRecord(ImageRecord *record)
{
  if (m_record)
    delete m_record;

  m_record = record;

  if (!m_record) {
    m_state = Downloading;
  }
  else if (record->width && record->height && record->size) {
    m_state = Ready;
  }
  else {
    m_state = Error;
  }
}
