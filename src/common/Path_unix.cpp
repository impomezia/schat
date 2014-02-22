/* $Id: Path_unix.cpp 3549 2013-03-02 23:44:00Z IMPOMEZIA $
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
#include <QFileInfo>
#include <QSettings>

#include "Path.h"
#include "sglobal.h"

QString Path::data(Scope scope)
{
  if (m_portable)
    return appDirPath();

  if (scope == UserScope) {
    QSettings s(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    return QFileInfo(s.fileName()).absolutePath() + LC('/') + app();
  }

  return SCHAT_PREFIX + LS("/share/") + app();
}


QStringList Path::plugins()
{
  QStringList out;
  out.append(data(SystemScope) + LS("/plugins/qt"));
  out.append(data(SystemScope) + LS("/plugins"));

# if defined(SCHAT_DAEMON)
  out.append(SCHAT_LIBDIR + LS("/schatd2/plugins"));
# else
  out.append(SCHAT_LIBDIR + LS("/schat2/plugins"));
# endif

  if (!isPortable())
    out.append(data() + LS("/plugins"));

  return out;
}
