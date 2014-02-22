/* $Id: authd.cpp 3732 2013-07-07 18:55:44Z IMPOMEZIA $
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
#include <QStringList>
#include <QTextCodec>

#include "AuthCore.h"
#include "sglobal.h"
#include "tools/CmdLine.h"
#include "version.h"

#if defined(Q_OS_LINUX)
#include <stdio.h>
#include <unistd.h>
#endif

int main(int argc, char *argv[])
{
# if defined(Q_OS_LINUX)
  if (argc > 1) {
    char d[] = "-D";
    for (int count = 1; count < argc; count++) {
      if (strncmp(argv[count], d, 2) == 0)
        daemon(0, 0);
    }
  }
# endif

  QCoreApplication app(argc, argv);
# if QT_VERSION < 0x050000
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
# endif
  app.setApplicationName(SCHAT_NAME);
  app.setApplicationVersion(SCHAT_VERSION);
  app.setOrganizationName(LS("IMPOMEZIA"));
  app.setOrganizationDomain(SCHAT_DOMAIN);

  CmdLine::init(app.arguments());
  AuthCore::version();

  if (CmdLine::has(QStringList() << LS("--version") << LS("-version")))
    return 0;

  AuthCore core;
  return app.exec();
}
