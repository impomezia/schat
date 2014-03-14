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

#include <QTextCodec>
#include <QTimer>

#if defined(Q_OS_WIN)
# include <QFileInfo>
# include <QProcess>
#endif

#include "ChatApp.h"
#include "ChatCore.h"
#include "ChatSettings.h"
#include "Path.h"
#include "sglobal.h"
#include "ui/ChatWindow.h"
#include "version.h"

ChatApp::ChatApp(int &argc, char **argv)
  : QtSingleApplication(argc, argv)
  , m_core(0)
  , m_window(0)
{
  setApplicationName(SCHAT_NAME);
  setApplicationVersion(SCHAT_VERSION);
  setOrganizationName(LS("IMPOMEZIA"));
  setOrganizationDomain(SCHAT_DOMAIN);
  setQuitOnLastWindowClosed(false);

# if defined(Q_OS_LINUX)
  setAttribute(Qt::AA_DontShowIconsInMenus, false);
# endif
# if QT_VERSION < 0x050000
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
# endif

  QPalette palette = this->palette();
  palette.setColor(QPalette::Inactive, QPalette::Highlight, palette.color(QPalette::Highlight));
  setPalette(palette);

# if QT_VERSION >= 0x050000 && defined(Q_OS_OSX)
  setAttribute(Qt::AA_UseHighDpiPixmaps);
# endif

# if !defined(SCHAT_NO_SINGLEAPP)
  connect(this, SIGNAL(messageReceived(QString)), SLOT(handleMessage(QString)));
# endif
}


ChatApp::~ChatApp()
{
  if (m_window)
    delete m_window;
}


bool ChatApp::isRunning()
{
# if defined(SCHAT_NO_SINGLEAPP)
  return false;
# else
  QStringList args = arguments();
  args.removeFirst();

  if (args.isEmpty())
    return sendMessage(QString());

  const QString message = args.join(LS(", "));

  if (args.contains(CMDLINE_ARG_EXIT)) {
    sendMessage(message);
    return true;
  }

  if (args.contains(CMDLINE_ARG_EXEC) || args.contains(CMDLINE_ARG_BASE))
    return false;

  return sendMessage(message);
# endif
}


void ChatApp::start()
{
  const QStringList args = arguments();
  Path::initWithBase(value(CMDLINE_ARG_BASE, args));

  m_core = new ChatCore(this);
  if (!ChatCore::settings()->value(SETTINGS_LABS_DISABLE_UI).toBool()) {
    m_window = new ChatWindow();
    connect(m_window, SIGNAL(restartRequest()), SLOT(restart()));

    if (args.contains(CMDLINE_ARG_HIDE))
      m_window->hide();
    else
      m_window->showChat();
  }
}


void ChatApp::stop()
{
  if (m_window)
    delete m_window;

  if (m_core)
    delete m_core;

  m_core = 0;
  m_window = 0;
}


#if defined(Q_OS_WIN)
bool ChatApp::selfUpdate()
{
  if (QString(LS(SCHAT_PLATFORM)).isEmpty() || QFileInfo(applicationFilePath()).baseName() != LS("schat2"))
    return false;

  const QString appPath = QApplication::applicationDirPath();
  QSettings s(appPath + LS("/schat2.conf"), QSettings::IniFormat);
  s.setIniCodec("UTF-8");
  s.beginGroup(LS("Update"));
  if (s.value(LS("Ready"), false) != true)
    return false;

  s.setValue(LS("Ready"), false);
  const QString version = s.value(LS("Version"), QString()).toString();
  if (version.isEmpty())
    return false;

  int revision = s.value(LS("Revision"), 0).toInt();
  if (revision < 1)
    return false;

  if (SCHAT_VER_PATH >= revision)
    return false;

  const QString file = appPath + LS("/.schat2/schat2-") + version + LS(".exe");
  if (!QFile::exists(file))
    return false;

  QProcess::startDetached(LC('"') + file + LC('"'), QStringList(LS("-update")), appPath);
  return true;
}
#endif


void ChatApp::handleMessage(const QString& message)
{
# if !defined(SCHAT_NO_SINGLEAPP)
  const QStringList args = message.split(LS(", "));
  if (!m_window)
    return;

  if (args.contains(CMDLINE_ARG_EXIT)) {
    m_window->closeChat();
    return;
  }

  QTimer::singleShot(0, m_window, SLOT(showChat()));
# endif
}


void ChatApp::restart()
{
  stop();

# if defined(Q_OS_WIN)
  if (selfUpdate()) {
    quit();
    return;
  }
# endif

  start();
}


QString ChatApp::value(const QString &cmdKey, const QStringList &arguments) const
{
  const int index = arguments.indexOf(cmdKey);
  if (index == -1 || index + 1 == arguments.size())
    return QString();

  return arguments.at(index + 1);
}
