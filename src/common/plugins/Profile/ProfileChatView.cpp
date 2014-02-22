/* $Id: ProfileChatView.cpp 2509 2012-04-07 20:43:44Z IMPOMEZIA $
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
#include "ProfileChatView.h"
#include "ui/tabs/ChatView.h"
#include "sglobal.h"

ProfileChatView::ProfileChatView(QObject *parent)
  : ChatViewHooks(parent)
{
}


void ProfileChatView::initImpl(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) == SimpleID::UserId)
    view->addJS(LS("qrc:/js/Profile/Profile.js"));
}


void ProfileChatView::loadFinishedImpl(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) == SimpleID::UserId)
    view->evaluateJavaScript(LS("Loader.loadCSS('qrc:/css/flags.css');"));
}
