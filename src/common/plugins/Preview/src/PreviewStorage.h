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

#ifndef PREVIEWSTORAGE_H_
#define PREVIEWSTORAGE_H_

#include <QObject>

#include "id/ChatId.h"
#include "interfaces/IDownloadItem.h"

class PreviewDB;
class PreviewItem;

class PreviewStorage : public QObject
{
  Q_OBJECT

public:
  PreviewStorage(QObject *parent = 0);
  ~PreviewStorage();
  QList<ChatId> findByOID(const ChatId &id) const;
  void add(const ChatId &messageId, const QList<QUrl> &urls);

private slots:
  void onFinished(DownloadItem item);

private:
  PreviewDB *m_db;
  QMap<ChatId, PreviewItem*> m_items;
  QMap<ChatId, QList<ChatId> > m_messages;
};

#endif // PREVIEWSTORAGE_H
