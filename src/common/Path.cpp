/* $Id: Path.cpp 3549 2013-03-02 23:44:00Z IMPOMEZIA $
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
#include <QDir>
#include <QFileInfo>
#include <QSettings>

#include "Path.h"
#include "sglobal.h"

bool Path::m_portable = false;
QString Path::m_app;
QString Path::m_appDirPath;

void Path::init(const QString &app)
{
  m_appDirPath = QCoreApplication::applicationDirPath();
  m_app = app.isEmpty() ? QFileInfo(QCoreApplication::applicationFilePath()).baseName() : app;

  QSettings s(appDirPath() + LC('/') + Path::app() + LS(".init"), QSettings::IniFormat);
  s.setIniCodec("UTF-8");
  m_portable = s.value(LS("Portable"), false).toBool();

  QDir().mkpath(data());
  QDir().mkpath(cache());

  foreach (const QString &path, plugins()) {
    QCoreApplication::addLibraryPath(path);
  }
}


void Path::initWithBase(const QString &base)
{
  if (base.isEmpty() || base.contains(LC('.')) || base.contains(LC('/')) || base.contains(LC('\\')))
    return init();

  QSettings s(appDirPath() + LC('/') + Path::app() + LS(".init"), QSettings::IniFormat);
  s.setIniCodec("UTF-8");
  m_portable = s.value(LS("Portable"), false).toBool();

  m_appDirPath = QCoreApplication::applicationDirPath();
  m_app        = base;

  QDir().mkpath(data());
  QDir().mkpath(cache());

  foreach (const QString &path, plugins()) {
    QCoreApplication::addLibraryPath(path);
  }
}


QString Path::app()
{
  if (m_app.isEmpty())
    m_app = QFileInfo(QCoreApplication::applicationFilePath()).baseName();

  return m_app;
}


QString Path::appDirPath()
{
  if (m_appDirPath.isEmpty())
    m_appDirPath = QCoreApplication::applicationDirPath();

  return m_appDirPath;
}


/*!
 * Возвращает папку для размещения кешей.
 */
QString Path::cache()
{
  if (isPortable())
    return data() + LS("/.") + app();

  return data();
}


/*!
 * Возвращает имя конфигурационного файла.
 */
QString Path::config()
{
  return data() + LC('/') + app() + LS(".conf");
}


QString Path::file(const QString &path, const QString &name)
{
  if (name.isEmpty())
    return name;

  if (!QFileInfo(name).isRelative())
    return name;

  return QDir::cleanPath(path + LC('/') + name);
}
