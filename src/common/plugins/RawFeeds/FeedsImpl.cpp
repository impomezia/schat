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
#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "FeedsImpl.h"
#include "messages/AlertMessage.h"
#include "net/NetReply.h"
#include "net/packets/FeedNotice.h"
#include "RawFeedsMessage.h"
#include "RawFeedsPlugin_p.h"
#include "ui/TabWidget.h"

namespace Hooks
{

FeedsImpl::FeedsImpl(RawFeeds *parent)
  : Feeds(parent)
  , m_plugin(parent)
{
  ChatClient::feeds()->hooks()->add(this);
}


void FeedsImpl::onReply(const NetRequest &req, const NetReply &reply)
{
  if (!m_plugin->remove(req.id))
    return;

  AlertMessage::show(reply.toJSON(), ALERT_MESSAGE_INFO, ChatCore::currentId());
}


void FeedsImpl::readFeedImpl(const FeedNotice &packet)
{
  if (!m_plugin->isEnabled())
    return;

  if (TabWidget::i()) {
    RawFeedsMessage message(packet);
    TabWidget::add(message, false);
  }
}

} // namespace Hooks
