/* $Id: PrivateTab.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QVBoxLayout>

#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "messages/ServiceMessage.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/ChatView.h"
#include "ui/tabs/FindWidget.h"
#include "ui/tabs/PrivateTab.h"
#include "ui/TabWidget.h"

PrivateTab::PrivateTab(ClientChannel channel, TabWidget *parent)
  : ChannelBaseTab(channel, LS("talk"), parent)
{
  QVBoxLayout *mainLay = new QVBoxLayout(this);
  mainLay->addWidget(m_chatView);
  mainLay->setMargin(0);
  mainLay->setSpacing(0);

  setText(channel->name());

  ChatClient::channels()->join(id());

  connect(ChatClient::channels(), SIGNAL(channel(ChannelInfo)), SLOT(channel(ChannelInfo)));
  connect(ChatClient::channels(), SIGNAL(quit(QByteArray)), SLOT(quit(QByteArray)));
  connect(ChatClient::i(), SIGNAL(online()), SLOT(online()));
}


void PrivateTab::layoutFindWidget()
{
  QVBoxLayout *mainLay = qobject_cast<QVBoxLayout *>(layout());
  if (mainLay)
    mainLay->insertWidget(0, m_findWidget);
}


void PrivateTab::channel(const ChannelInfo &info)
{
  if (info.id() != id())
    return;

  if (m_joined)
    return;

  m_chatView->add(ServiceMessage::joined(id()));
  m_joined = true;
}


void PrivateTab::online()
{
  if (m_joined)
    return;

  m_chatView->add(ServiceMessage::joined(id()));
  m_joined = true;
}


void PrivateTab::quit(const QByteArray &user)
{
  if (id() != user)
    return;

  reload();

  m_chatView->add(ServiceMessage::quit(user));
  m_joined = false;
}
