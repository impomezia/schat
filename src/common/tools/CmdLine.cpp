/* $Id: CmdLine.cpp 3733 2013-07-07 20:50:31Z IMPOMEZIA $
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

#include <QCoreApplication>
#include <QFile>

#include "sglobal.h"
#include "tools/CmdLine.h"

QStringList CmdLine::m_args;


bool CmdLine::has(const QString &key)
{
  return m_args.contains(key);
}


bool CmdLine::has(const QStringList &keys)
{
  foreach (const QString &key, keys) {
    if (m_args.contains(key))
      return true;
  }

  return false;
}


QString CmdLine::pidFile(const QString &base)
{
  if (base.isEmpty())
    return QString();

  const QString file = value(QStringList() << LS("--pid") << LS("-P")).toString();

# if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
  if (file.isEmpty())
    return LS("/var/run/") + base + LS(".pid");
# endif

  return file;
}


QVariant CmdLine::value(const QString &key)
{
  if (key.isEmpty())
    return QVariant();

  int index = m_args.indexOf(key);
  if (index + 1 == m_args.size())
    return QVariant();

  if (index == -1) {
    if (key.size() > 2) {
      index = m_args.indexOf(QRegExp(key + LS("=*"), Qt::CaseSensitive, QRegExp::Wildcard));
      if (index != -1)
        return m_args.at(index).mid(key.size() + 1);
    }

    return QVariant();
  }

  return m_args.at(index + 1);
}


QVariant CmdLine::value(const QStringList &keys)
{
  if (keys.isEmpty())
    return QVariant();

  QVariant out;
  foreach (const QString &key, keys) {
    out = value(key);
    if (!out.isNull())
      return out;
  }

  return out;
}


void CmdLine::createPid(const QString &base)
{
  const QString file = pidFile(base);
  if (!file.isEmpty()) {
    QFile pid(file);
    if (pid.open(QIODevice::WriteOnly))
      pid.write(QByteArray::number(QCoreApplication::applicationPid()));
  }
}


void CmdLine::removePid(const QString &base)
{
  const QString pid = CmdLine::pidFile(base);
  if (!pid.isEmpty())
    QFile::remove(pid);
}
