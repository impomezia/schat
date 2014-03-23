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

#include "hooks/ChatViewHooks.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/tabs/ChatView.h"
#include "YouTubeChatView.h"

YouTubeChatView::YouTubeChatView(QObject *parent)
  : QObject(parent)
{
  ChatViewHooks::add(this);
}


YouTubeChatView::~YouTubeChatView()
{
  ChatViewHooks::remove(this);
}



void YouTubeChatView::init(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) == SimpleID::ServerId)
    return;

  view->addJS(LS("qrc:/js/YouTube/YouTube.js"));
}


void YouTubeChatView::loadFinished(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) == SimpleID::ServerId)
    return;

  view->evaluateJavaScript(LS("Loader.loadCSS('qrc:/css/YouTube/YouTube.css');"));
}
