/* $Id: YouTubePlugin.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QtPlugin>
#include <QWebSettings>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "sglobal.h"
#include "Translation.h"
#include "YouTubeChatView.h"
#include "YouTubeFilter.h"
#include "YouTubePlugin.h"
#include "YouTubePlugin_p.h"
#include "YouTubeSettings.h"

YouTubePluginImpl::YouTubePluginImpl(QObject *parent)
  : ChatPlugin(parent)
  , m_key(LS("YouTube/EmbedVideo"))
{
  ChatSettings *settings = ChatCore::settings();
  settings->setDefault(m_key, false);
  if (settings->value(m_key).toBool())
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);

  ChatCore::translation()->addOther(LS("youtube"));
  TokenFilter::add(LS("channel"), new YouTubeFilter());

  connect(settings, SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));
}


void YouTubePluginImpl::chatReady() Q_DECL_OVERRIDE
{
  new YouTubeChatView(this);
}


void YouTubePluginImpl::settingsChanged(const QString &key, const QVariant &value)
{
  if (key == m_key && value == true)
    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
}


ChatPlugin *YouTubePlugin::create()
{
  m_plugin = new YouTubePluginImpl(this);
  return m_plugin;
}


QWidget *YouTubePlugin::settings(QWidget *parent)
{
  return new YouTubeSettings(parent);
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(YouTube, YouTubePlugin);
#endif
