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
#include "ProfileChatView.h"
#include "sglobal.h"
#include "ui/tabs/ChatView.h"

ProfileChatView::ProfileChatView(QObject *parent)
  : QObject(parent)
{
  ChatViewHooks::add(this);
}


ProfileChatView::~ProfileChatView()
{
  ChatViewHooks::remove(this);
}


void ProfileChatView::init(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) == SimpleID::UserId)
    view->addJS(LS("qrc:/js/Profile/Profile.js"));
}


void ProfileChatView::loadFinished(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) == SimpleID::UserId)
    view->evaluateJavaScript(LS("Loader.loadCSS('qrc:/css/flags.css');"));
}
