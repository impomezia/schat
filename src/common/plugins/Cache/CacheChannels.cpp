/* $Id: CacheChannels.cpp 2104 2012-01-06 22:59:29Z IMPOMEZIA $
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

#include <QDebug>

#include "CacheChannels.h"
#include "CacheDB.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"

namespace Hooks
{

CacheChannels::CacheChannels(QObject *parent)
  : Channels(parent)
{
  ChatClient::channels()->hooks()->add(this);
}


ClientChannel CacheChannels::get(const QByteArray &id)
{
  return CacheDB::channel(id);
}


void CacheChannels::add(ClientChannel channel, const ChannelInfo &info, const QVariantMap &json)
{
  Q_UNUSED(info)
  Q_UNUSED(json)

  CacheDB::add(channel);
}

} // namespace Hooks
