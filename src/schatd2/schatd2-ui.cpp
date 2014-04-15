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

#ifdef SCHAT_NO_SINGLEAPP
# define QtSingleApplication QApplication
# include <QApplication>
#else
# include "qtsingleapplication/qtsingleapplication.h"
#endif

#include <QTextCodec>
#include <QStringList>

#include "sglobal.h"
#include "ui/DaemonUi.h"
#include "version.h"

#if defined(Q_OS_WIN) && defined(Q_CC_MSVC)
# include "ExceptionHandler.h"
#endif

int main(int argc, char *argv[])
{
# if defined(Q_OS_WIN) && defined(Q_CC_MSVC)
  initExceptionHandler();
# endif

  QtSingleApplication app(argc, argv);
# if QT_VERSION < 0x050000
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
# endif
  app.setApplicationName(SCHAT_NAME);
  app.setApplicationVersion(SCHAT_VERSION);
  app.setOrganizationName(LS("IMPOMEZIA"));
  app.setOrganizationDomain(SCHAT_DOMAIN);
  app.setQuitOnLastWindowClosed(false);

  QStringList args = app.arguments();
  args.removeFirst();

# ifndef SCHAT_NO_SINGLEAPP
  if (args.isEmpty() && app.sendMessage(QString()))
    return 0;

  const QString message = args.join(LS(", "));

  if (args.contains(LS("-exit"))) {
    app.sendMessage(message);
    return 0;
  }

  if (app.sendMessage(message))
    return 0;
# endif

  DaemonUi ui;
  if (args.contains(LS("-hide")))
    ui.hide();
  else
    ui.show();

# ifndef SCHAT_NO_SINGLEAPP
  QObject::connect(&app, SIGNAL(messageReceived(QString)), &ui, SLOT(handleMessage(QString)));
# endif

  return app.exec();
}
