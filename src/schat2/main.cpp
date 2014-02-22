/* $Id: main.cpp 3272 2012-11-16 23:59:04Z IMPOMEZIA $
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

#include "ChatApp.h"

int main(int argc, char *argv[])
{
  ChatApp app(argc, argv);
  if (app.isRunning())
    return 0;

# if defined(Q_OS_WIN32)
  if (ChatApp::selfUpdate())
    return 0;
# endif

  app.start();
  return app.exec();
}
