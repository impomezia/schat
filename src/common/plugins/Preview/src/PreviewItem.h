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

#ifndef PREVIEWITEM_H_
#define PREVIEWITEM_H_

#include "id/ChatId.h"
#include "interfaces/IDownloadItem.h"

struct ImageRecord;

class PreviewItem
{
public:
  enum State {
    Downloading,
    Ready,
    Error
  };

  PreviewItem(const QUrl &url);
  ~PreviewItem();
  inline const ChatId &id() const    { return m_id; }
  inline const QUrl &url() const     { return m_url; }
  inline ImageRecord *record() const { return m_record; }
  inline State state() const         { return m_state; }
  void setDownloadItem(DownloadItem item);
  void setRecord(ImageRecord *record);

private:
  ChatId m_id;
  QWeakPointer<IDownloadItem> m_item;
  ImageRecord *m_record;
  QUrl m_url;
  State m_state;
};

#endif // PREVIEWITEM_H_
