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

#include <QDragEnterEvent>
#include <QMimeData>
#include <QNetworkDiskCache>
#include <QWebFrame>

#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "hooks/ChatViewHooks.h"
#include "Path.h"
#include "Providers.h"
#include "sglobal.h"
#include "ShareChatView.h"
#include "ShareCore.h"
#include "ShareDnD.h"
#include "ui/tabs/ChatView.h"

ShareChatView::ShareChatView(ShareCore *core)
  : QObject(core)
  , m_core(core)
{
  ChatViewHooks::add(this);
}


ShareChatView::~ShareChatView()
{
  ChatViewHooks::remove(this);
}


bool ShareChatView::dragEnterEvent(ChatView *view, QDragEnterEvent *event)
{
  m_id.init(view->id());
  if (!event->mimeData()->hasUrls() || !isAcceptable())
    return false;

  IProvider *provider = m_core->providers()->current();

  if (ShareDnD::getFiles(event->mimeData(), provider->maxSize(), provider->maxImages()).isEmpty() && ShareDnD::getUrls(event->mimeData(), provider->maxImages()).isEmpty())
    return false;

  event->acceptProposedAction();
  return true;
}


bool ShareChatView::dropEvent(ChatView *view, QDropEvent *event)
{
  m_id.init(view->id());
  if (!event->mimeData()->hasUrls() || !isAcceptable())
    return false;

  IProvider *provider = m_core->providers()->current();
  if (!provider)
    return false;

  const QList<QUrl> urls = ShareDnD::getFiles(event->mimeData(), provider->maxSize(), provider->maxImages());
  if (urls.isEmpty())
    return false;

  event->acceptProposedAction();
  m_core->openFile(urls.first().toLocalFile());
  return true;
}


bool ShareChatView::isAcceptable() const
{
  if (m_id.type() != ChatId::ChannelId)
    return false;

  ClientChannel channel = ChatClient::channels()->get(m_id.toByteArray());
  if (!channel)
    return false;

  return channel->feed("info")->data().value("images", true) != false;
}
