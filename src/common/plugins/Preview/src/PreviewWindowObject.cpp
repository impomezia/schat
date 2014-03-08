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

#include "PreviewCore.h"
#include "PreviewStorage.h"
#include "PreviewWindowObject.h"

PreviewWindowObject::PreviewWindowObject(PreviewCore *core)
  : QObject(core)
  , m_core(core)
{
}


QVariant PreviewWindowObject::findByOID(const QString &id) const
{
  if (id.size() != ObjectId::kEncodedSize)
    return QVariant();

  const QList<ChatId> ids = m_core->storage()->findByOID(id);
  if (ids.isEmpty())
    return QVariant();

  QStringList out;

  foreach (const ChatId &id, ids) {
    out.append(id.toString());
  }

  return out;
}
