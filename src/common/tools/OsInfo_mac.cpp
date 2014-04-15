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

#include "OsInfo.h"
#include "sglobal.h"

void OsInfo::init()
{
  QString os;
  m_type = MacOSX;

  switch (QSysInfo::MacintoshVersion) {
    case QSysInfo::QSysInfo::MV_MAVERICKS:
      os = LS("OS X 10.9 Mavericks");
      break;

#   if QT_VERSION >= 0x040803
    case QSysInfo::MV_MOUNTAINLION:
      os = LS("OS X 10.8 Mountain Lion");
      break;
#   endif

    case QSysInfo::MV_LION:
      os = LS("OS X 10.7 Lion");
      break;

    case QSysInfo::MV_SNOWLEOPARD:
      os = LS("Mac OS X 10.6 Snow Leopard");
      break;

    case QSysInfo::MV_LEOPARD:
      os = LS("Mac OS X 10.5 Leopard");
      break;

    default:
      os = LS("OS X");
      break;
  }

  m_json.insert(LS("os"), os);
}
