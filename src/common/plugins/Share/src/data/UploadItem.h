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

#ifndef UPLOADITEM_H_
#define UPLOADITEM_H_

#include <QSharedPointer>

#include "data/Thumbnail.h"
#include "id/ChatId.h"

class UploadItem
{
public:
  enum { Type };

  UploadItem(int mode = -1);
  virtual ~UploadItem();
  inline const ChatId& id() const                      { return m_id; }
  inline const ChatId& roomId() const                  { return m_roomId; }
  inline const Thumbnail& thumbnail() const            { return m_thumbnail; }
  inline int mode() const                              { return m_mode; }
  inline qint64 date() const                           { return m_date; }
  inline QString title() const                         { return m_title; }
  inline virtual int type() const                      { return Type; }
  inline void setThumbnail(const Thumbnail &thumbnail) { m_thumbnail = thumbnail; }
  inline void setTitle(const QString &title)           { m_title = title; }
  virtual bool isNull() const;
  virtual QString toString() const;

protected:
  void init();

  ChatId m_id;           ///< Идентификатор итема.
  ChatId m_roomId;       ///< Идентификатор комнаты.
  int m_mode;            ///< Опциональный режим работы.
  qint64 m_date;         ///< Дата создания.
  QString m_title;       ///< Описание.
  Thumbnail m_thumbnail; ///< Миниатюра.
};


typedef QSharedPointer<UploadItem> UploadItemPtr;


template <class T> inline T uploaditem_cast(UploadItem *item)
{
  return int(static_cast<T>(0)->Type) == int(UploadItem::Type) || (item && int(static_cast<T>(0)->Type) == item->type()) ? static_cast<T>(item) : 0;
}


template <class T> inline T uploaditem_cast(const UploadItem *item)
{
  return int(static_cast<T>(0)->Type) == int(UploadItem::Type) || (item && int(static_cast<T>(0)->Type) == item->type()) ? static_cast<T>(item) : 0;
}

#endif /* UPLOADITEM_H_ */
