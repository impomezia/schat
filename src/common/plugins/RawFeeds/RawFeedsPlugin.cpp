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

#include <QtPlugin>

#include "FeedsImpl.h"
#include "hooks/ChatViewHooks.h"
#include "RawFeedsCmd.h"
#include "RawFeedsPlugin.h"
#include "RawFeedsPlugin_p.h"
#include "sglobal.h"
#include "ui/tabs/ChatView.h"

RawFeeds::RawFeeds(QObject *parent)
  : ChatPlugin(parent)
  , m_enabled(false)
{
  new Hooks::RawFeedsCmd(this);
  new Hooks::FeedsImpl(this);

  connect(ChatViewHooks::i(), SIGNAL(initHook(ChatView*)), SLOT(init(ChatView*)));
  connect(ChatViewHooks::i(), SIGNAL(loadFinishedHook(ChatView*)), SLOT(loadFinished(ChatView*)));
}


void RawFeeds::init(ChatView *view)
{
  view->addJS(LS("qrc:/js/RawFeeds/RawFeeds.js"));
}


void RawFeeds::loadFinished(ChatView *view)
{
  view->addCSS(LS("qrc:/css/RawFeeds/RawFeeds.css"));
}


ChatPlugin *RawFeedsPlugin::create()
{
  m_plugin = new RawFeeds(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(RawFeeds, RawFeedsPlugin);
#endif
