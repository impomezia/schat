/* $Id: PopupPlugin.cpp 3146 2012-10-10 16:59:51Z IMPOMEZIA $
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

#include <QtPlugin>
#include <QVBoxLayout>

#include "ChatCore.h"
#include "PopupManager.h"
#include "PopupPlugin.h"
#include "PopupPlugin_p.h"
#include "PopupSettings.h"
#include "sglobal.h"
#include "Translation.h"
#include "ui/tabs/SettingsTabHook.h"

PopupPluginImpl::PopupPluginImpl(QObject *parent)
  : ChatPlugin(parent)
{
  m_manager = new PopupManager(this);
  ChatCore::translation()->addOther(LS("popup"));

  connect(SettingsTabHook::i(), SIGNAL(added(QString,SettingsPage*)), SLOT(added(QString,SettingsPage*)));
}


void PopupPluginImpl::added(const QString &id, SettingsPage *page)
{
  if (id != LS("alerts"))
    return;

  page->mainLayout()->addWidget(new PopupSettings(page));
}


ChatPlugin *PopupPlugin::create()
{
  m_plugin = new PopupPluginImpl(this);
  return m_plugin;
}


#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(Popup, PopupPlugin);
#endif
