/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#ifndef ISETTINGS_H_
#define ISETTINGS_H_

#include <QVariant>

class ISettings
{
public:
  virtual ~ISettings() {}

  virtual QVariant defaultValue(const QString &key) const = 0;
  virtual QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const = 0;
  virtual void setDefault(const QString &key, const QVariant &value) = 0;
  virtual void setValue(const QString &key, const QVariant &value, bool notify = true) = 0;
};

#endif // ISETTINGS_H_
