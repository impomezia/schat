/* $Id: ConsolePlugin.cpp 3775 2013-08-24 03:41:07Z IMPOMEZIA $
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

#include <QApplication>
#include <QMenu>
#include <QTimer>
#include <QtPlugin>

#include "ChatCore.h"
#include "ConsoleCmd.h"
#include "ConsolePlugin.h"
#include "ConsolePlugin_p.h"
#include "sglobal.h"
#include "Tr.h"
#include "Translation.h"
#include "ui/ConsoleTab.h"
#include "ui/TabsToolBar.h"
#include "ui/TabWidget.h"

class ConsoleTr : public Tr
{
  Q_DECLARE_TR_FUNCTIONS(ConsoleTr)

public:
  ConsoleTr() : Tr() { m_prefix = LS("console_"); }

protected:
  QString valueImpl(const QString &key) const
  {
    if      (key == LS("bad_server"))           return tr("Remote management is disabled by default for security reasons. <a href='https://wiki.schat.me/How_to_enable_console'>How to enable?</a>");
    else if (key == LS("password"))             return tr("Password:");
    else if (key == LS("login"))                return tr("Login");
    else if (key == LS("empty_password"))       return tr("Password cannot be empty");
    else if (key == LS("incorect_password"))    return tr("You entered an incorrect password");
    else if (key == LS("change_password"))      return tr("Change password");
    else if (key == LS("new_password"))         return tr("New password:");
    else if (key == LS("confirm_new_password")) return tr("Confirm new password:");
    else if (key == LS("save"))                 return tr("Save");
    else if (key == LS("cancel"))               return tr("Cancel");
    else if (key == LS("password_mismatch"))    return tr("The passwords you entered don't match");
    else if (key == LS("password_short"))       return tr("Password is too short");
    else if (key == LS("home"))                 return tr("Home");
    else if (key == LS("logout"))               return tr("Logout");
    else if (key == LS("server_name"))          return tr("Server Name");
    else if (key == LS("about_server"))         return tr("About server");
    else if (key == LS("name"))                 return tr("Name:");
    else if (key == LS("version"))              return tr("Version:");
    else if (key == LS("cert_expired"))         return tr("Certificate has expired");
    else if (key == LS("users_online"))         return tr("Users online");
    else if (key == LS("now"))                  return tr("Now:");
    else if (key == LS("peak"))                 return tr("Peak:");
    else if (key == LS("auth"))                 return tr("Registration");
    else if (key == LS("oauth"))                return tr("OAuth");
    else if (key == LS("anonymous"))            return tr("Anonymous");
    else if (key == LS("ok"))                   return tr("OK");
    else if (key == LS("main_channel"))         return tr("Main channel");
    else if (key == LS("use_main_channel"))     return tr("Use main channel");
    else if (key == LS("forced_join"))          return tr("Forced join to main channel");
    else if (key == LS("disable_leave"))        return tr("Disable leave from channel");
    return QString();
  }
};


ConsolePluginImpl::ConsolePluginImpl(QObject *parent)
  : ChatPlugin(parent)
  , m_console(0)
{
  m_tr = new ConsoleTr();
  new ConsoleCmd(this);

  ChatCore::translation()->addOther(LS("console"));

  QTimer::singleShot(0, this, SLOT(start()));
}


ConsolePluginImpl::~ConsolePluginImpl()
{
  delete m_tr;
}


void ConsolePluginImpl::show()
{
  TabWidget::i()->tab(CONSOLE_TAB);
}


void ConsolePluginImpl::showMenu(QMenu *menu, QAction *separator)
{
  Q_UNUSED(separator)

  m_console->setText(tr("Console"));
  m_console->setChecked(TabWidget::isCurrent(TabWidget::i()->tab(CONSOLE_TAB, TabWidget::NoOptions)));

  menu->addAction(m_console);
}


void ConsolePluginImpl::start()
{
  TabWidget *tabs = TabWidget::i();
  if (!tabs)
    return;

  m_console = new QAction(this);
  m_console->setIcon(QIcon(LS(":/images/Console/terminal.png")));
  m_console->setCheckable(true);

  tabs->add(new ConsoleTabCreator());

  connect(tabs->toolBar(), SIGNAL(showMenu(QMenu*,QAction*)), SLOT(showMenu(QMenu*,QAction*)));
  connect(m_console, SIGNAL(triggered(bool)), SLOT(show()));
}


ChatPlugin *ConsolePlugin::create()
{
  m_plugin = new ConsolePluginImpl(this);
  return m_plugin;
}


#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(Console, ConsolePlugin);
#endif

