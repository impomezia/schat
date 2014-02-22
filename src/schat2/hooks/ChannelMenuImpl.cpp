/* $Id: ChannelMenuImpl.cpp 3280 2012-11-20 17:26:19Z IMPOMEZIA $
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

#include <QMenu>

#include "ChatCore.h"
#include "ChatUrls.h"
#include "hooks/ChannelMenuImpl.h"
#include "net/SimpleID.h"
#include "ui/ChatIcons.h"
#include "sglobal.h"

ChannelMenuImpl::ChannelMenuImpl(QObject *parent)
  : ChannelMenu(parent)
  , m_info(0)
  , m_talk(0)
{
  add(this);
}


void ChannelMenuImpl::bindImpl(QMenu *menu, ClientChannel channel, Hooks::Scope scope)
{
  Q_UNUSED(scope)

  if (channel->type() != SimpleID::ChannelId)
    return;

  bool active = ChatCore::currentId() == channel->id();
  int page = channel->data().value(LS("page")).toInt();

  if (!active || (active && page != 0)) {
    m_talk = new QAction(SCHAT_ICON(Channel), tr("Talk..."), this);
    m_talk->setData(ChatUrls::toUrl(channel, LS("open")));
    menu->addAction(m_talk);
  }

  if (!active || (active && page != 1)) {
    m_info = new QAction(SCHAT_ICON(InfoBalloon), tr("Information..."), this);
    m_info->setData(ChatUrls::toUrl(channel, LS("info")));
    menu->addAction(m_info);
  }
}


void ChannelMenuImpl::cleanupImpl()
{
  if (m_info) delete m_info; m_info = 0;
  if (m_talk) delete m_talk; m_talk = 0;
}
