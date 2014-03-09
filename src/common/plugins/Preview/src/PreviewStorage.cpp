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

#include "PreviewDB.h"
#include "PreviewStorage.h"
#include "Path.h"
#include "sglobal.h"
#include "PreviewItem.h"
#include "ChatCore.h"
#include "NetworkAccess.h"

PreviewStorage::PreviewStorage(QObject *parent) :
  QObject(parent)
{
  m_db = new PreviewDB(this);
  m_db->open(Path::cache() + LS("/preview.sqlite"));

  connect(ChatCore::networkAccess(), SIGNAL(finished(DownloadItem)), SLOT(onFinished(DownloadItem)));
}


PreviewStorage::~PreviewStorage()
{
  qDeleteAll(m_items);
}


PreviewItem *PreviewStorage::findById(const ChatId &id) const
{
  return m_items.value(id);
}


QList<ChatId> PreviewStorage::findByOID(const ChatId &id) const
{
  return m_messages.value(id);
}


void PreviewStorage::add(const ChatId &messageId, const QList<QUrl> &urls)
{
  QList<ChatId> ids;

  foreach (const QUrl &url, urls) {
    PreviewItem *item = new PreviewItem(url);
    const ChatId &id  = item->id();

    if (!ids.contains(id))
      ids.append(id);

    if (m_items.contains(id)) {
      delete item;
      continue;
    }

    m_items.insert(id, item);

    item->setRecord(m_db->findById(id));
    if (item->state() == PreviewItem::Downloading) {
      item->setDownloadItem(ChatCore::networkAccess()->download(item->url(), QDir::tempPath() + LS("/") + id.toString()));
    }
  }

  if (!ids.isEmpty())
    m_messages.insert(messageId.oid(), ids);
}


void PreviewStorage::onFinished(DownloadItem item)
{
  PreviewItem *i = m_items.value(ChatId(item->url().toEncoded(), ChatId::NormalizedId));
  if (!i)
    return;

  if (item->error())
    return downloadError(i);
}


void PreviewStorage::downloadError(PreviewItem *item)
{
  item->setState(PreviewItem::Error);
  m_db->save(item->id(), item->url());

  emit changed(item->id());
}
