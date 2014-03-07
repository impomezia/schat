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

#include <QWebFrame>

#include "hooks/ChatViewHooks.h"
#include "PreviewChatView.h"
#include "PreviewCore.h"
#include "sglobal.h"
#include "ui/tabs/ChatView.h"
#include "PreviewWindowObject.h"

PreviewChatView::PreviewChatView(PreviewCore *core)
  : QObject(core)
  , m_core(core)
{
  ChatViewHooks::add(this);
}


PreviewChatView::~PreviewChatView()
{
  ChatViewHooks::remove(this);
}


void PreviewChatView::init(ChatView *view)
{
  if (ChatId(view->id()).type() == ChatId::ServerId)
    return;

  view->page()->mainFrame()->addToJavaScriptWindowObject(LS("PreviewPlugin"), m_core->windowObject());
  view->addJS(LS("qrc:/js/Preview/Preview.js"));
}


void PreviewChatView::loadFinished(ChatView *view)
{
  if (ChatId(view->id()).type() == ChatId::ServerId)
    return;

  view->addCSS(LS("qrc:/css/Preview/Preview.css"));
}
