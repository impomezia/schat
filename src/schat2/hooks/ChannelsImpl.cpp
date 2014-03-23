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

#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/SimpleClient.h"
#include "hooks/ChannelsImpl.h"
#include "sglobal.h"
#include "ui/TabWidget.h"

namespace Hooks
{

ChannelsImpl::ChannelsImpl(QObject *parent)
  : Channels(parent)
{
  ChatClient::channels()->hooks()->add(this);
}


ClientChannel ChannelsImpl::get(const QByteArray &id)
{
  if (!TabWidget::i())
    return ClientChannel();

  return TabWidget::i()->channel(id);
}


void ChannelsImpl::add(ClientChannel channel, const ChannelInfo &info, const QVariantMap &json)
{
  Q_UNUSED(json)

  if (info.id() == ChatClient::id()) {
    ChatCore::settings()->setValue(ChatSettings::kProfileNick,   ChatClient::io()->nick());
    ChatCore::settings()->setValue(ChatSettings::kProfileGender, channel->gender().raw());

    if (channel->status().value() != Status::AutoAway)
      ChatCore::settings()->setValue(ChatSettings::kProfileStatus, channel->status().value());
  }

  if (info.id() == ChatClient::serverId() && info.option() & ChannelInfo::Renamed)
    ChatNotify::start(Notify::ServerRenamed);
}

} // namespace Hooks
