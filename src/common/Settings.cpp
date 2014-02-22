/* $Id: Settings.cpp 2610 2012-05-06 00:04:20Z IMPOMEZIA $
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

#include "Settings.h"

Settings::Settings(const QString &fileName, QObject *parent)
  : QSettings(fileName, QSettings::IniFormat, parent)
{
  setIniCodec("UTF-8");
}


QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
  if (defaultValue.isValid())
    m_default[key] = defaultValue;

  return QSettings::value(key, m_default.value(key));
}


void Settings::setDefault(const QString &key, const QVariant &value)
{
  m_default[key] = value;
}


void Settings::setValue(const QString &key, const QVariant &value, bool notify)
{
  if (QSettings::value(key, m_default.value(key)) == value)
    return;

  if (m_default.contains(key) && m_default.value(key) == value)
    QSettings::remove(key);
  else
    QSettings::setValue(key, value);

  if (notify)
    emit changed(key, value);
}
