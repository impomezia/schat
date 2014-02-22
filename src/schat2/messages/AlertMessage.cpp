/* $Id: AlertMessage.cpp 3713 2013-06-25 23:56:26Z IMPOMEZIA $
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

#include "ChatCore.h"
#include "messages/AlertMessage.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/tabs/ChannelBaseTab.h"
#include "ui/tabs/ChatView.h"
#include "ui/tabs/ServerTab.h"
#include "ui/TabWidget.h"

AlertMessage::AlertMessage(const QString &text, const QString &extra)
  : Message(ChatCore::randomId(), ChatCore::currentId(), LS("alert"), LS("addAlertMessage"))
{
  m_data.insert(kText, text);
  m_data.insert(kExtra, LS("alert-box-") + extra);
}


QString AlertMessage::show(const Message &message)
{
  if (!TabWidget::i() || !message.isValid())
    return QString();

  if (SimpleID::typeOf(message.tab()) == SimpleID::ServerId) {
    TabWidget::i()->serverTab()->chatView()->add(message);
    return message.id();
  }

  ChannelBaseTab *tab = TabWidget::i()->channelTab(message.tab(), false, false);
  if (!tab)
    return QString();

  tab->chatView()->add(message);
  return message.id();
}


QString AlertMessage::show(const QString &text, const QString &extra, const QByteArray &tab)
{
  AlertMessage message(text, extra);
  if (!tab.isEmpty())
    message.m_tab = tab;

  return show(message);
}
