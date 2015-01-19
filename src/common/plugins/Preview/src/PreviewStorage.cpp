/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QThreadPool>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "NetworkAccess.h"
#include "Path.h"
#include "PreviewCore.h"
#include "PreviewDB.h"
#include "PreviewItem.h"
#include "PreviewRunnable.h"
#include "PreviewStorage.h"
#include "sglobal.h"

PreviewStorage::PreviewStorage(QObject *parent) :
  QObject(parent)
{
  qRegisterMetaType<ImageRecord>("ImageRecord");

  m_mimes.append(LS("image/bmp"));
  m_mimes.append(LS("image/gif"));
  m_mimes.append(LS("image/jpeg"));
  m_mimes.append(LS("image/png"));

  m_db = new PreviewDB(this);
  m_db->open(Path::cache() + LS("/preview.sqlite"));

  connect(ChatCore::networkAccess(), SIGNAL(finished(DownloadItem)), SLOT(onFinished(DownloadItem)));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(onNotify(Notify)));
}


PreviewStorage::~PreviewStorage()
{
  qDeleteAll(m_items);
  QThreadPool::globalInstance()->waitForDone();
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

    ids.append(id);

    if (m_items.contains(id)) {
      delete item;
      continue;
    }

    m_items.insert(id, item);

    item->setRecord(m_db->findById(id));
    if (item->state() == PreviewItem::Downloading) {
      QVariantMap options;
      options.insert(LS("limit"), ChatCore::settings()->value(PreviewCore::kMaxSize));
      options.insert(LS("mimes"), m_mimes);

      item->setDownloadItem(ChatCore::networkAccess()->download(item->url(), QDir::tempPath() + LS("/") + id.toString(), options));
    }
  }

  if (!ids.isEmpty())
    m_messages.insert(messageId.oid(), ids);
}


void PreviewStorage::onFinished(const ImageRecord &record)
{
  PreviewItem *item = m_items.value(record.id);
  if (!item)
    return;

  item->setRecord(record);
  m_db->save(item);
  emit changed(item->id());
}


void PreviewStorage::onFinished(DownloadItem item)
{
  PreviewItem *i = m_items.value(ChatId(item->url().toEncoded(), ChatId::NormalizedId));
  if (!i)
    return;

  if (item->error()) {
    i->setState(PreviewItem::Error);
    m_db->save(i);

    emit changed(i->id());
    return;
  }

  PreviewRunnable *task = new PreviewRunnable(i->id().toString());
  connect(task, SIGNAL(finished(ImageRecord)), SLOT(onFinished(ImageRecord)));

  QThreadPool::globalInstance()->start(task);
}


void PreviewStorage::onNotify(const Notify &notify)
{
  if (notify.type() == Notify::ClearCache) {
    removeRecursively(PreviewItem::path());
    m_db->clear();
  }
}


void PreviewStorage::removeRecursively(const QString &path)
{
  QDirIterator di(path, QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
  while (di.hasNext()) {
    di.next();
    const QFileInfo &fi = di.fileInfo();

    if (fi.isDir() && !fi.isSymLink())
      removeRecursively(di.filePath());
    else
      QFile::remove(di.filePath());
  }

  QDir().rmdir(path);
  QApplication::processEvents();
}
