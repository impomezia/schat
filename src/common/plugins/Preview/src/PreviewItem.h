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

  enum Flags {
    NoFlags,
    Animated
  };

  enum FileType {
    Original,
    Thumbnail
  };

  PreviewItem(const QUrl &url);
  inline const ChatId &id() const          { return m_id; }
  inline const QString &format()           { return m_format; }
  inline const QUrl &url() const           { return m_url; }
  inline int flags() const                 { return m_flags; }
  inline int height() const                { return m_height; }
  inline int size() const                  { return m_size; }
  inline int width() const                 { return m_width; }
  inline State state() const               { return m_state; }
  inline void setState(State state)        { m_state = state; }
  QString fileName(FileType type) const;
  void setDownloadItem(DownloadItem item);
  void setRecord(const ImageRecord &record);

  static QString path();

private:
  ChatId m_id;
  int m_flags;
  int m_height;
  int m_size;
  int m_width;
  QString m_format;
  QUrl m_url;
  QWeakPointer<IDownloadItem> m_item;
  State m_state;

  static QString m_path;
};

#endif // PREVIEWITEM_H_
