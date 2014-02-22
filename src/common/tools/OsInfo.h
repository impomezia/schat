/* $Id: OsInfo.h 3663 2013-05-10 13:19:09Z IMPOMEZIA $
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

#ifndef OSINFO_H_
#define OSINFO_H_

#include <QVariant>

#include "schat.h"

class SCHAT_EXPORT OsInfo
{
public:
  ///< Тип операционной системы.
  enum OsType {
    Unknown  = 0,  ///< Неизвестная операционная система.
    Windows  = 1,  ///< Windows, диапазон от 1 до 19.
    MacOSX   = 20, ///< Mac OS X, диапазон от 20 до 39.
    Linux    = 40, ///< Linux, диапазон от 40 до 79.
    Ubuntu   = 41, ///< Ubuntu.
    Gentoo   = 42, ///< Gentoo Linux.
    Debian   = 43, ///< Debian GNU/Linux.
    OpenSUSE = 44, ///< openSUSE.
    Fedora   = 45, ///< Fedora.
    Android  = 79  ///< Android.
  };

  static int type();
  static QVariantMap json();

private:
  OsInfo() {}
  static void init();

# if defined(Q_OS_LINUX)
  static void detectLinux(QString &name);
# endif

  static int m_type;         ///< Тип операционной системы.
  static QVariantMap m_json; ///< JSON данные.
};

#endif /* OSINFO_H_ */
