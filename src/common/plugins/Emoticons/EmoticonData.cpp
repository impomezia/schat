/* $Id: EmoticonData.cpp 3489 2013-02-10 17:05:32Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#include <QFile>
#include <QVariant>

#include "EmoticonData.h"
#include "QtEscape.h"
#include "sglobal.h"

EmoticonData::EmoticonData(const QString &file, const QString &id, const QVariantMap &data)
  : m_hidden(false)
  , m_height(0)
  , m_width(0)
  , m_file(file)
  , m_id(id)
{
  if (data.isEmpty())
    return;

  QVariantList icon = data.value(LS("icon")).toList();
  if (icon.size() < 4)
    return;

  m_file   += LC('/') + icon.at(0).toString();
  m_width  = icon.at(1).toInt();
  m_height = icon.at(2).toInt();
  m_hidden = data.value(LS("hidden"), false).toBool();

  for (int i = 3; i < icon.size(); ++i) {
    const QString text = Qt::escape(icon.at(i).toString());
    if (text.isEmpty())
      continue;

    if (!m_texts.contains(text))
      m_texts.append(text);
  }
}


bool EmoticonData::isValid() const
{
  if (!QFile::exists(m_file))
    return false;

  if (m_width <= 0)
    return false;

  if (m_height <= 0)
    return false;

  if (m_texts.isEmpty())
    return false;

  return true;
}
