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

#include <QCoreApplication>
#include <CoreServices/CoreServices.h>

#include "OsInfo.h"
#include "sglobal.h"

QByteArray OsInfo::userAgent()
{
  if (!m_ua.isEmpty())
    return m_ua;

  SInt32 major, minor, bugfix;
  Gestalt(gestaltSystemVersionMajor, &major);
  Gestalt(gestaltSystemVersionMinor, &minor);
  Gestalt(gestaltSystemVersionBugFix, &bugfix);

  const QString os = QString("Macintosh; Intel Mac OS X %1_%2_%3").arg(major).arg(minor).arg(bugfix);

  QString ua = QString(LS("Mozilla/5.0 (%1) Qt/%2 %3/%4"))
      .arg(os)
      .arg(qVersion())
      .arg(QCoreApplication::applicationName())
      .arg(QCoreApplication::applicationVersion());

  m_ua = ua.toLatin1();
  return m_ua;
}


void OsInfo::init()
{
  QString os;
  m_type = MacOSX;

  SInt32 major, minor, bugfix;
  Gestalt(gestaltSystemVersionMajor, &major);
  Gestalt(gestaltSystemVersionMinor, &minor);
  Gestalt(gestaltSystemVersionBugFix, &bugfix);

  switch (minor) {
    case 9:
      os = QString(LS("OS X 10.9.%1 Mavericks")).arg(bugfix);
      break;

    case 8:
      os = QString(LS("OS X 10.8.%1 Mountain Lion")).arg(bugfix);;
      break;

    case 7:
      os = QString(LS("OS X 10.7.%1 Lion")).arg(bugfix);;
      break;

    case 6:
      os = QString(LS("Mac OS X 10.6.%1 Snow Leopard")).arg(bugfix);;
      break;

    default:
      os = QString(LS("OS X %1.%2.%3")).arg(major).arg(minor).arg(bugfix);;
      break;
  }

  m_json.insert(LS("os"), os);
}
