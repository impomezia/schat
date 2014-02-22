/* $Id: Normalize.cpp 2146 2012-01-14 17:16:10Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#include "Channel.h"
#include "net/SimpleID.h"
#include "Normalize.h"
#include "sglobal.h"

QHash<QChar, QChar> Normalize::m_map;

QByteArray Normalize::toId(Channel *channel)
{
  return Normalize::toId(channel->type(), channel->name());
}


QByteArray Normalize::toId(const QString &text)
{
  return SimpleID::make(toString(text).toUtf8(), SimpleID::NormalizedId);
}


QByteArray Normalize::toId(int type, const QString &name)
{
  return Normalize::toId((type == SimpleID::UserId ? LC('~') : LC('#')) + name);
}


QString Normalize::toString(const QString &text)
{
  QString out = text.toLower().simplified();
  if (out.isEmpty())
    return out;

  for (int i = 0; i < out.size(); ++i) {
    if (m_map.contains(out.at(i)))
      out[i] = m_map.value(out.at(i));
  }

  return out;
}


void Normalize::init()
{
  m_map[0x0430] = 'a'; // а
  m_map[0x0435] = 'e'; // е
  m_map[0x0451] = 'e'; // ё
  m_map[0x043C] = 'm'; // м
  m_map[0x0440] = 'p'; // р
  m_map[0x0441] = 'c'; // с
  m_map[0x043E] = 'o'; // о
  m_map[0x0443] = 'y'; // у
  m_map[0x0445] = 'x'; // х
  m_map['l']    = 'i'; // l
}
