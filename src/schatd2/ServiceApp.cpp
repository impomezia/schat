/* $Id: ServiceApp.cpp 3732 2013-07-07 18:55:44Z IMPOMEZIA $
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

#include <QTextCodec>

#include "NodeInit.h"
#include "ServiceApp.h"
#include "sglobal.h"
#include "version.h"

ServiceApp::ServiceApp(int argc, char **argv)
  : QtService<QCoreApplication>(argc, argv, LS("Simple Chat 2 Daemon"))
{
# if QT_VERSION < 0x050000
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
# endif

  setServiceDescription(LS("IMPOMEZIA Simple Chat Daemon"));
  setStartupType(QtServiceController::AutoStartup);
}


void ServiceApp::start()
{
  QCoreApplication *app = application();
  app->setApplicationName(SCHAT_NAME);
  app->setApplicationVersion(SCHAT_VERSION);
  app->setOrganizationName(LS("IMPOMEZIA"));
  app->setOrganizationDomain(SCHAT_DOMAIN);

  m_init = new NodeInit(LS("schatd2"), app);
}


void ServiceApp::stop()
{
  application()->quit();
  if (m_init)
    m_init->quit();
}
