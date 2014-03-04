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

#include <QMenu>

#include "ChannelsMenuImpl.h"
#include "ChannelsPlugin_p.h"
#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientFeeds.h"
#include "feeds/AclFeed.h"
#include "feeds/InfoFeed.h"
#include "hooks/ChannelMenu.h"
#include "hooks/MessagesImpl.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/ChannelBaseTab.h"
#include "ui/tabs/ChatView.h"
#include "ui/TabWidget.h"

ChannelsMenuImpl::ChannelsMenuImpl(QObject *parent)
  : QObject(parent)
  , m_advanced(0)
  , m_ignore(0)
  , m_kick(0)
  , m_ro(0)
  , m_permissions(0)
{
  ChannelMenu::add(this);
}


ChannelsMenuImpl::~ChannelsMenuImpl()
{
  ChannelMenu::remove(this);
}


bool ChannelsMenuImpl::trigger(QAction *action)
{
  if (action == m_ignore) {
    ChannelsPluginImpl::ignore(m_id, action->isChecked());
  }
  else if (action == m_ro || action == m_rw || action == m_forbidden) {
    ChannelsPluginImpl::setAcl(m_id, ChatCore::currentId(), action->data().toInt());
  }
  else if (action == m_advanced) {
    ChannelBaseTab *tab = TabWidget::i()->channelTab(ChatCore::currentId());
    if (tab)
      tab->chatView()->evaluateJavaScript(LS("Channels.editAcl(\"") + SimpleID::encode(action->data().toByteArray()) + LS("\");"));
  }
  else if (action == m_kick) {
    ClientFeeds::post(ChatCore::currentId(), ACL_FEED_KICK_REQ, SimpleID::encode(m_id));
  }
  else
    return false;

  return true;
}


void ChannelsMenuImpl::bind(QMenu *menu, ClientChannel channel, IChannelMenu::Scope scope)
{
  if (channel->type() != SimpleID::UserId)
    return;

  m_id   = channel->id();
  m_self = m_id == ChatClient::id();

  if (scope == IChannelMenu::UserViewScope || scope == IChannelMenu::ChatViewScope)
    permissions(menu, channel);

  if (!m_self)
    invite(menu, channel);

  if (ChatCore::settings()->value(SETTINGS_CHANNELS_IGNORING).toBool() && !m_self) {
    if (!m_permissions)
      menu->addSeparator();

    m_ignore = menu->addAction(SCHAT_ICON(Prohibition), tr("Ignore"));
    m_ignore->setCheckable(true);
    m_ignore->setChecked(Hooks::MessagesImpl::isIgnored(channel));
  }
}


void ChannelsMenuImpl::cleanup()
{
  m_advanced = 0;
  m_ignore = 0;
  m_permissions = 0;
  m_invite = 0;
  m_ro = 0;
  m_rw = 0;
  m_forbidden = 0;
  m_kick = 0;
  m_id.clear();
}


void ChannelsMenuImpl::invite(QAction *action)
{
  if (!action)
    return;

  const QVariantList data = action->data().toList();
  if (data.size() != 2)
    return;

  ChannelsPluginImpl::inviteTo(data.at(0).toByteArray(), data.at(1).toByteArray());
}


bool ChannelsMenuImpl::canInviteTo(ClientChannel user, ClientChannel channel)
{
  if (SimpleID::typeOf(channel->id()) == SimpleID::ChannelId && channel->channels().contains(ChatClient::id()) && !channel->channels().contains(user->id()) && user->status() != Status::Offline) {
    const int match = ClientFeeds::match(channel, user);
    if (match & Acl::Read)
      return true;

    if (!(match & Acl::Write)) {
      const int acl = ClientFeeds::match(channel, ChatClient::channel());
      if ((acl & Acl::Edit) || (acl & Acl::SpecialWrite))
        return true;

      FeedPtr feed = channel->feed(FEED_NAME_INFO, false);
      return (feed && feed->data().value(INFO_FEED_SUDO_KEY).toBool() && !channel->feed(FEED_NAME_ACL)->data().contains(SimpleID::encode(user->id())));
    }

    return true;
  }

  return false;
}


/*!
 * Формирование меню приглашения в канал.
 */
void ChannelsMenuImpl::invite(QMenu *menu, ClientChannel user)
{
  QList<ClientChannel> list;
  const QMap<QByteArray, ClientChannel>& channels = ChatClient::channels()->channels();

  QMapIterator<QByteArray, ClientChannel> i(channels);
  while (i.hasNext()) {
    i.next();

    if (canInviteTo(user, i.value()))
      list.append(i.value());
  }

  if (list.isEmpty())
    return;

  m_invite = menu->addMenu(ChatIcons::icon(ChatIcons::icon(user, ChatIcons::NoOptions), LS(":/images/add-small.png")), tr("Invite to"));
  foreach (const ClientChannel &channel, list) {
    m_invite->addAction(SCHAT_ICON(Channel), channel->name())->setData(QVariantList() << user->id() << channel->id());
  }

  connect(m_invite, SIGNAL(triggered(QAction*)), SLOT(invite(QAction*)));
}


/*!
 * Формирование меню прав доступа к каналу.
 */
void ChannelsMenuImpl::permissions(QMenu *menu, ClientChannel user)
{
  ClientChannel channel = ChatClient::channels()->get(ChatCore::currentId());
  if (!channel || channel->type() != SimpleID::ChannelId)
    return;

  const int acl = ClientFeeds::match(channel, ChatClient::channel());
  if (acl == -1)
    return;

  const bool moderator = !m_self && (acl & Acl::Edit || acl & Acl::SpecialWrite);
  const bool advanced  = (acl & Acl::Edit) && ChatClient::id() != user->id();

  if (moderator || advanced) {
    menu->addSeparator();
    m_permissions = menu->addMenu(SCHAT_ICON(Key), tr("Permissions"));

    if (moderator) {
      QActionGroup *group = new QActionGroup(m_permissions);
      const int acl = ClientFeeds::match(channel, user);

      m_rw = m_permissions->addAction(tr("Read write"));
      m_rw->setCheckable(true);
      m_rw->setChecked(acl == (Acl::Read | Acl::Write));
      m_rw->setData(Acl::Read | Acl::Write);

      m_ro = m_permissions->addAction(tr("Read only"));
      m_ro->setCheckable(true);
      m_ro->setChecked(acl == Acl::Read);
      m_ro->setData(Acl::Read);

      m_forbidden = m_permissions->addAction(tr("No access"));
      m_forbidden->setCheckable(true);
      m_forbidden->setChecked(acl == 0);
      m_forbidden->setData(0);

      group->addAction(m_rw);
      group->addAction(m_ro);
      group->addAction(m_forbidden);
    }

    if (advanced) {
      m_permissions->addSeparator();
      m_advanced = m_permissions->addAction(SCHAT_ICON(Gear), tr("Advanced..."));
      m_advanced->setData(user->id());
    }

    if (moderator && user->status() == Status::Offline)
      m_kick = menu->addAction(SCHAT_ICON(Remove), tr("Remove"));
  }
}
