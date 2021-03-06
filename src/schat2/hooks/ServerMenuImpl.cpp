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

#include "ChatCore.h"
#include "ChatUrls.h"
#include "hooks/ChannelMenu.h"
#include "hooks/ServerMenuImpl.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"

ServerMenuImpl::ServerMenuImpl(QObject *parent)
  : QObject(parent)
  , m_alerts(0)
  , m_info(0)
{
  ChannelMenu::add(this);
}


bool ServerMenuImpl::trigger(QAction *action)
{
  Q_UNUSED(action)

  return false;
}


void ServerMenuImpl::bind(QMenu *menu, ClientChannel channel, Scope scope)
{
  Q_UNUSED(scope)

  if (channel->type() != SimpleID::ServerId)
    return;

  bool active = ChatCore::currentId() == channel->id();
  int page = channel->data().value(LS("page")).toInt();

  if (!active || (active && page != 0)) {
    m_alerts = new QAction(SCHAT_ICON(Balloon), tr("Notifications..."), this);
    m_alerts->setData(ChatUrls::toUrl(channel, LS("open")));
    menu->addAction(m_alerts);
  }

  if (!active || (active && page != 1)) {
    m_info = new QAction(SCHAT_ICON(Information), tr("Information..."), this);
    m_info->setData(ChatUrls::toUrl(channel, LS("info")));
    menu->addAction(m_info);
  }
}


void ServerMenuImpl::cleanup()
{
  if (m_alerts) delete m_alerts; m_alerts = 0;
  if (m_info) delete m_info; m_info = 0;
}
