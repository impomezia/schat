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
#include <QFile>
#include <QSettings>
#include <sys/utsname.h>

#include "OsInfo.h"
#include "sglobal.h"

QByteArray OsInfo::userAgent()
{
  if (!m_ua.isEmpty())
    return m_ua;

  QString os = LS("X11; ");

  if (type() == Ubuntu)
    os += LS("Ubuntu; ");

#if defined(Q_OS_LINUX)
# if defined(__x86_64__)
    os += LS("Linux x86_64");
# elif defined(__i386__)
    os += LS("Linux i686");
# else
    os += LS("Linux");
# endif
#endif

  QString ua = QString(LS("Mozilla/5.0 (%1) Qt/%2 %3/%4"))
      .arg(os)
      .arg(qVersion())
      .arg(QCoreApplication::applicationName())
      .arg(QCoreApplication::applicationVersion());

  m_ua = ua.toLatin1();
  return m_ua;
}


void detectLinux(QString &name, int &type)
{
  if (QFile::exists(LS("/etc/lsb-release"))) {
    QSettings s(LS("/etc/lsb-release"), QSettings::IniFormat);
    s.setIniCodec("UTF-8");
    const QString id = s.value(LS("DISTRIB_ID")).toString();
    if (id == LS("Ubuntu"))
      type = OsInfo::Ubuntu;

    name = s.value(LS("DISTRIB_DESCRIPTION")).toString();
    return;
  }

  if (QFile::exists(LS("/etc/gentoo-release"))) {
    type = OsInfo::Gentoo;
    QFile file(LS("/etc/gentoo-release"));
    if (file.open(QIODevice::ReadOnly))
      name = file.read(128).trimmed();

    return;
  }

  if (QFile::exists(LS("/etc/debian_version"))) {
    type = OsInfo::Debian;
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
      type = OsInfo::OpenSUSE;
    else if (id == LS("fedora"))
      type = OsInfo::Fedora;

    name = s.value(LS("PRETTY_NAME")).toString();
    return;
  }

  if (QFile::exists(LS("/etc/fedora-release"))) {
    type = OsInfo::Fedora;
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


void OsInfo::init()
{
  QString os;
  m_type = Linux;

  detectLinux(os, m_type);
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

  m_json.insert(LS("os"), os);
}
