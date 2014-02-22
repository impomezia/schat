/* $Id: ProxyPlugin.cpp 3159 2012-10-12 21:02:04Z IMPOMEZIA $
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

#include <QVBoxLayout>
#include <QtPlugin>

#include "ChatCore.h"
#include "ProxyPlugin.h"
#include "ProxyPlugin_p.h"
#include "ProxySettings.h"
#include "sglobal.h"
#include "Translation.h"
#include "ui/tabs/SettingsTabHook.h"
#include "ChatSettings.h"

ProxyPluginImpl::ProxyPluginImpl(QObject *parent)
  : ChatPlugin(parent)
{
  ChatSettings *settings = ChatCore::settings();
  const QString prefix = LS("Proxy/");

  settings->setLocalDefault(prefix + LS("Type"),            LS("none"));
  settings->setLocalDefault(prefix + LS("http.host"),       QString());
  settings->setLocalDefault(prefix + LS("http.port"),       3128);
  settings->setLocalDefault(prefix + LS("http.name"),       QString());
  settings->setLocalDefault(prefix + LS("http.password"),   QString());
  settings->setLocalDefault(prefix + LS("socks5.host"),     QString());
  settings->setLocalDefault(prefix + LS("socks5.port"),     1080);
  settings->setLocalDefault(prefix + LS("socks5.name"),     QString());
  settings->setLocalDefault(prefix + LS("socks5.password"), QString());

  ChatCore::translation()->addOther(LS("proxy"));

  if (settings->value(prefix + LS("Type")) != LS("none"))
    ProxySettings::setProxy();

  connect(SettingsTabHook::i(), SIGNAL(added(QString,SettingsPage*)), SLOT(added(QString,SettingsPage*)));
}


void ProxyPluginImpl::added(const QString &id, SettingsPage *page)
{
  if (id != LS("network"))
    return;

  page->mainLayout()->addWidget(new ProxySettings(page));
}


ChatPlugin *ProxyPlugin::create()
{
  m_plugin = new ProxyPluginImpl(this);
  return m_plugin;
}


#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(Proxy, ProxyPlugin);
#endif
