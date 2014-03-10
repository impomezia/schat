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
#include "PreviewCore.h"
#include "PreviewItem.h"
#include "PreviewStorage.h"
#include "PreviewWindowObject.h"
#include "sglobal.h"

PreviewWindowObject::PreviewWindowObject(PreviewCore *core)
  : QObject(core)
  , m_core(core)
{
  connect(core->storage(), SIGNAL(changed(ChatId)), SLOT(onChanged(ChatId)));
}


QVariant PreviewWindowObject::findById(const QString &id) const
{
  PreviewItem *item = m_core->storage()->findById(id);
  if (!item)
    return QVariant();

  QVariantMap map;
  map.insert(LS("id"),    item->id().toString());
  map.insert(LS("state"), item->state());
  map.insert(LS("url"),   item->url());

  if (item->state() == PreviewItem::Ready) {
    map.insert(LS("thumb"),  QUrl::fromLocalFile(item->fileName(PreviewItem::Thumbnail)));
    map.insert(LS("orig"),   QUrl::fromLocalFile(item->fileName(PreviewItem::Original)));
    map.insert(LS("flags"),  item->flags());
    map.insert(LS("width"),  item->width());
    map.insert(LS("height"), item->height());
  }

  return map;
}


QVariant PreviewWindowObject::findByOID(const QString &id) const
{
  const QList<ChatId> ids = m_core->storage()->findByOID(id);
  if (ids.isEmpty())
    return QVariant();

  QStringList out;

  foreach (const ChatId &id, ids) {
    out.append(id.toString());
  }

  return out;
}


void PreviewWindowObject::onChanged(const ChatId &id)
{
  emit changed(id.toString());
}
