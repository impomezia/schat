/* $Id: OsInfo.cpp 3773 2013-08-22 00:16:23Z IMPOMEZIA $
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

#include "sglobal.h"
#include "tools/OsInfo.h"

#if defined(Q_OS_UNIX)
# include <QFile>
# include <QSettings>
# include <sys/utsname.h>
#endif

int OsInfo::m_type = -1;
QVariantMap OsInfo::m_json;

int OsInfo::type()
{
  if (m_type == -1)
    init();

  return m_type;
}


QVariantMap OsInfo::json()
{
  if (m_type == -1)
    init();

  return m_json;
}


void OsInfo::init()
{
  QString os;
# if defined(Q_OS_WIN32)
  m_type = Windows;
  switch (QSysInfo::WindowsVersion) {
#   if QT_VERSION >= 0x040803
    case QSysInfo::WV_WINDOWS8:
      os = LS("Windows 8");
      break;
#   endif

    case QSysInfo::WV_WINDOWS7:
      os = LS("Windows 7");
      break;

    case QSysInfo::WV_VISTA:
      os = LS("Windows Vista");
      break;

    case QSysInfo::WV_2003:
      os = LS("Windows Server 2003");
      break;

    case QSysInfo::WV_XP:
      os = LS("Windows XP");
      break;

    case QSysInfo::WV_2000:
      os = LS("Windows 2000");
      break;

    default:
      os = LS("Windows");
      break;
  }
# elif defined(Q_OS_MAC)
  m_type = MacOSX;
  switch (QSysInfo::MacintoshVersion) {
#   if QT_VERSION >= 0x040803
    case QSysInfo::MV_MOUNTAINLION:
      os = LS("OS X 10.8 Mountain Lion");
      break;
#   endif

    case QSysInfo::MV_LION:
      os = LS("Mac OS X 10.7 Lion");
      break;

    case QSysInfo::MV_SNOWLEOPARD:
      os = LS("Mac OS X 10.6 Snow Leopard");
      break;

    case QSysInfo::MV_LEOPARD:
      os = LS("Mac OS X 10.5 Leopard");
      break;

    default:
      os = LS("Mac OS X");
      break;
  }
# elif defined(Q_OS_LINUX)
  m_type = Linux;
  detectLinux(os);
  if (os.isEmpty()) {
    utsname buf;
    if (uname(&buf) != -1) {
      os.append(buf.sysname).append(LC(' '));
      os.append(buf.release).append(LC(' '));
      os.append(buf.machine);
    }
    else {
      os = LS("Linux");
    }
  }
# else
  m_type = Unknown;
# endif

  m_json[LS("os")] = os;
}


#if defined(Q_OS_LINUX)
void OsInfo::detectLinux(QString &name)
{
  if (QFile::exists(LS("/etc/lsb-release"))) {
    QSettings s(LS("/etc/lsb-release"), QSettings::IniFormat);
    s.setIniCodec("UTF-8");
    const QString id = s.value(LS("DISTRIB_ID")).toString();
    if (id == LS("Ubuntu"))
      m_type = Ubuntu;

    name = s.value(LS("DISTRIB_DESCRIPTION")).toString();
    return;
  }

  if (QFile::exists(LS("/etc/gentoo-release"))) {
    m_type = Gentoo;
    QFile file(LS("/etc/gentoo-release"));
    if (file.open(QIODevice::ReadOnly))
      name = file.read(128).trimmed();

    return;
  }

  if (QFile::exists(LS("/etc/debian_version"))) {
    m_type = Debian;
    name = LS("Debian GNU/Linux");
    QFile file(LS("/etc/debian_version"));
    if (file.open(QIODevice::ReadOnly))
      name += LS(" ") + file.read(64).trimmed();

    return;
  }

  else if (QFile::exists(LS("/etc/os-release"))) {
    QSettings s(LS("/etc/os-release"), QSettings::IniFormat);
    s.setIniCodec("UTF-8");
    const QString id = s.value(LS("ID")).toString();
    if (id == LS("opensuse"))
      m_type = OpenSUSE;
    else if (id == LS("fedora"))
      m_type = Fedora;

    name = s.value(LS("PRETTY_NAME")).toString();
    return;
  }

  if (QFile::exists(LS("/etc/fedora-release"))) {
    m_type = Fedora;
    QFile file(LS("/etc/fedora-release"));
    if (file.open(QIODevice::ReadOnly))
      name = file.read(128).trimmed();

    return;
  }

  if (QFile::exists(LS("/etc/redhat-release"))) {
    QFile file(LS("/etc/redhat-release"));
    if (file.open(QIODevice::ReadOnly))
      name = file.read(128).trimmed();

    return;
  }
}
#endif
