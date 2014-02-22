/* $Id: SettingsHook.h 3003 2012-08-14 12:24:34Z IMPOMEZIA $
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

#ifndef SETTINGSHOOK_H_
#define SETTINGSHOOK_H_

#include "StorageHook.h"

class SettingsHook : public StorageHook
{
public:
  SettingsHook();
  QStringList keys() const;
  QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
};

#endif /* SETTINGSHOOK_H_ */
