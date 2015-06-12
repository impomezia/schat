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
#include <qt_windows.h>

#include "OsInfo.h"
#include "sglobal.h"

QByteArray OsInfo::userAgent()
{
  if (!m_ua.isEmpty())
    return m_ua;

  OSVERSIONINFO osvi;

  ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  GetVersionEx(&osvi);

  const QString os = QString("Windows NT %1.%2").arg(osvi.dwMajorVersion).arg(osvi.dwMinorVersion);

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
  m_type = Windows;

  switch (QSysInfo::WindowsVersion) {
#   if QT_VERSION >= 0x040806
    case QSysInfo::WV_WINDOWS8_1:
      os = LS("8.1");
      break;
#   endif

#   if QT_VERSION >= 0x040803
    case QSysInfo::WV_WINDOWS8:
      os = LS("8");
      break;
#   endif

    case QSysInfo::WV_WINDOWS7:
      os = LS("7");
      break;

    case QSysInfo::WV_VISTA:
      os = LS("Vista");
      break;

    case QSysInfo::WV_2003:
      os = LS("2003");
      break;

    case QSysInfo::WV_XP:
      os = LS("XP");
      break;

    case QSysInfo::WV_2000:
      os = LS("2000");
      break;

    default:
      break;
  }

  m_json.insert(LS("os"), os.isEmpty() ? LS("Windows") : LS("Windows ") + os);
}
