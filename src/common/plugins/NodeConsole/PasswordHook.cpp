/* $Id: PasswordHook.cpp 3212 2012-10-23 02:20:54Z IMPOMEZIA $
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

#include "DataBase.h"
#include "net/SimpleID.h"
#include "PasswordHook.h"
#include "sglobal.h"

PasswordHook::PasswordHook()
  : StorageHook()
{
}


bool PasswordHook::setValue(const QString &key, const QVariant &value)
{
  QString password = value.toString();
  if (password.size() != 34 || SimpleID::typeOf(SimpleID::decode(password)) != SimpleID::PasswordId)
    return false;

  DataBase::setValue(key, value);
  return true;
}


QStringList PasswordHook::keys() const
{
  return QStringList(LS("password"));
}


QVariant PasswordHook::value(const QString &key, const QVariant &defaultValue) const
{
  Q_UNUSED(key)
  Q_UNUSED(defaultValue)

  QString password = DataBase::value(LS("password")).toString();
  if (password.size() != 34 || SimpleID::typeOf(SimpleID::decode(password)) != SimpleID::PasswordId)
    password = LS("2AZ6EKXDJCXLKZQPYIKAV3BVQUGE3KMXOA");

  return password;
}
