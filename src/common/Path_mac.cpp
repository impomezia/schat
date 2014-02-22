/* $Id: Path_mac.cpp 3308 2012-12-09 14:21:37Z IMPOMEZIA $
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

#include <QDir>
#include <QFileInfo>

#include "Path.h"
#include "sglobal.h"

QString Path::data(Scope scope)
{
  if (scope == UserScope && !m_portable)
    return QDir::homePath() + LS("/Library/Application Support/IMPOMEZIA/") + app();

  return QDir::cleanPath(appDirPath() + LS("/../Resources"));
}


QStringList Path::plugins()
{
  QStringList out;
  out.append(QDir::cleanPath(appDirPath() + LS("/../PlugIns")) + LS("/qt"));
  out.append(QDir::cleanPath(appDirPath() + LS("/../PlugIns")));

  if (!isPortable())
    out.append(data() + LS("/plugins"));

  return out;
}
