/* $Id: ClientImpl.cpp 3664 2013-05-11 18:19:01Z IMPOMEZIA $
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
#include "ChatSettings.h"
#include "ChatUrls.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/SimpleClient.h"
#include "feeds/ServerFeed.h"
#include "hooks/ClientImpl.h"
#include "sglobal.h"

namespace Hooks
{

ClientImpl::ClientImpl(QObject *parent)
  : Client(parent)
  , m_setup(false)
{
  ChatClient::i()->hooks()->add(this);

  connect(ChatClient::channels(), SIGNAL(channel(QByteArray)), SLOT(channel(QByteArray)));
}


void ClientImpl::setup()
{
  m_setup = true;
}


void ClientImpl::channel(const QByteArray &id)
{
  if (m_setup && ChatClient::id() == id) {
    m_setup                  = false;
    ClientChannels *channels = ChatClient::channels();
    const int policy         = channels->policy();

    if (policy & ServerFeed::MainChannelPolicy && policy & ServerFeed::ForcedJoinPolicy) {
      const QByteArray id = channels->mainId();
      if (!channels->joined().contains(id))
        channels->join(id);
    }
  }
}

} // namespace Hooks
