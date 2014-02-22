/* $Id: ChannelTab.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QAction>
#include <QSplitter>
#include <QVBoxLayout>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/SimpleClient.h"
#include "hooks/ChannelMenu.h"
#include "messages/ServiceMessage.h"
#include "sglobal.h"
#include "ui/InputWidget.h"
#include "ui/tabs/ChannelTab.h"
#include "ui/tabs/ChatView.h"
#include "ui/tabs/FindWidget.h"
#include "ui/tabs/UserView.h"
#include "ui/TabWidget.h"

ChannelTab::ChannelTab(ClientChannel channel, TabWidget *parent)
  : ChannelBaseTab(channel, LS("channel"), parent)
{
  m_userView = new UserView(channel, this);

  m_leftLayout = new QVBoxLayout(this);
  m_leftLayout->addWidget(m_chatView);
  m_leftLayout->setMargin(0);
  m_leftLayout->setSpacing(0);

  QWidget *left = new QWidget(this);
  left->setLayout(m_leftLayout);

  m_splitter = new QSplitter(this);
  m_splitter->addWidget(left);
  m_splitter->addWidget(m_userView);
  m_splitter->setStretchFactor(0, 1);
  m_splitter->setStretchFactor(1, 1);
  m_splitter->setOpaqueResize(false);

  QVBoxLayout *mainLay = new QVBoxLayout(this);
  mainLay->addWidget(m_splitter);
  mainLay->setMargin(0);
  mainLay->setSpacing(0);

  setText(channel->name());

  connect(ChatClient::channels(), SIGNAL(channels(QList<QByteArray>)), SLOT(channels(QList<QByteArray>)));
  connect(ChatClient::channels(), SIGNAL(joined(QByteArray,QByteArray)), SLOT(joined(QByteArray,QByteArray)));
  connect(ChatClient::channels(), SIGNAL(part(QByteArray,QByteArray)), SLOT(part(QByteArray,QByteArray)));
  connect(ChatClient::channels(), SIGNAL(quit(QByteArray,bool)), SLOT(quit(QByteArray,bool)));
  connect(ChatClient::channels(), SIGNAL(channel(QByteArray)), SLOT(channel(QByteArray)));

  m_chatView->add(ServiceMessage::joined(ChatClient::id()));
}


bool ChannelTab::isActive() const
{
  return m_userView->contains(ChatClient::id());
}


void ChannelTab::setOnline(bool online)
{
  if (!online)
    m_userView->clear();

  ChannelBaseTab::setOnline(online);
}


void ChannelTab::layoutFindWidget()
{
  m_leftLayout->insertWidget(0, m_findWidget);
}


void ChannelTab::channel(const QByteArray &id)
{
  if (this->id() != id)
    return;

  if (m_joined)
    return;

  m_chatView->add(ServiceMessage::joined(ChatClient::id()));
  m_joined = true;
}


/*!
 * Обработка получения данных о новых каналах.
 * Если канал содержит пользователя, но ещё не был добавлен в список пользователей, то он будет добавлен.
 *
 * \param channels Список идентификаторов каналов.
 */
void ChannelTab::channels(const QList<QByteArray> &channels)
{
  m_userView->setSortable(false);
  foreach (const QByteArray &id, channels) {
    if (c()->channels().all().contains(id) && !m_userView->contains(id))
      m_userView->add(ChatClient::channels()->get(id));
  }

  m_userView->sort();
}


/*!
 * Обработка входа пользователя в канал.
 */
void ChannelTab::joined(const QByteArray &channel, const QByteArray &user)
{
  if (id() != channel)
    return;

  m_userView->add(ChatClient::channels()->get(user));

  if (ChatClient::id() != user)
    m_chatView->add(ServiceMessage::joined(user));
}


void ChannelTab::part(const QByteArray &channel, const QByteArray &user)
{
  if (id() != channel)
    return;

  if (m_userView->remove(user))
    m_chatView->add(ServiceMessage::part(user));

  if (ChatClient::id() == user)
    m_userView->clear();
}


void ChannelTab::quit(const QByteArray &user, bool offline)
{
  if (!c()->channels().all().contains(user))
    return;

  if (offline)
    m_userView->reload(user);
  else
    m_userView->remove(user);

  m_chatView->add(ServiceMessage::quit(user));
}
