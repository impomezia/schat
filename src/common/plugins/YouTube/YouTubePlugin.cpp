/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

YouTubePluginImpl::YouTubePluginImpl(QObject *parent)
  : ChatPlugin(parent)
{
  ChatCore::translation()->addOther(LS("youtube"));
  TokenFilter::add(LS("channel"), new YouTubeFilter());

  QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
}


void YouTubePluginImpl::chatReady()
{
  new YouTubeChatView(this);
}


ChatPlugin *YouTubePlugin::create()
{
  m_plugin = new YouTubePluginImpl(this);
  return m_plugin;
}

Q_EXPORT_PLUGIN2(Preview, YouTubePlugin);
