/* $Id: YouTubeChatView.cpp 2682 2012-05-20 09:50:39Z IMPOMEZIA $
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

#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/tabs/ChatView.h"
#include "YouTubeChatView.h"

YouTubeChatView::YouTubeChatView(QObject *parent)
  : ChatViewHooks(parent)
{
}


void YouTubeChatView::initImpl(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) == SimpleID::ServerId)
    return;

  view->addJS(LS("qrc:/js/YouTube/YouTube.js"));
}


void YouTubeChatView::loadFinishedImpl(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) == SimpleID::ServerId)
    return;

  view->evaluateJavaScript(LS("Loader.loadCSS('qrc:/css/YouTube/YouTube.css');"));
}
