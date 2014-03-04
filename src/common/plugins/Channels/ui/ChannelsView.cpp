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

#include <QWebFrame>
#include <QContextMenuEvent>
#include <QMenu>

#include "ChatNotify.h"
#include "ChatUrls.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientFeeds.h"
#include "feeds/AclFeed.h"
#include "feeds/InfoFeed.h"
#include "hooks/ChannelMenu.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/AddRoomDialog.h"
#include "ui/ChannelsView.h"
#include "ui/tabs/ChatView.h"
#include "ui/TabWidget.h"
#include "WebBridge.h"

ChannelsView::ChannelsView(QWidget *parent)
  : WebView(parent)
{
  connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(populateJavaScriptWindowObject()));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
  connect(ChatClient::channels(), SIGNAL(channel(QByteArray)), SLOT(channel(QByteArray)));

  retranslateUi();
}


QString ChannelsView::toUrl(const QString &id, const QString &name) const
{
  return LS("chat://channel/") + id + LS("/open?name=") + ChatId::toBase32(name.toUtf8()) + LS("&gender=0");
}


void ChannelsView::addRoom()
{
  AddRoomDialog *dialog = new AddRoomDialog(this);

# if QT_VERSION >= 0x050000
  connect(dialog, &AddRoomDialog::create, this, &ChannelsView::create);
# else
  connect(dialog, SIGNAL(create(QString,QString,bool)), SLOT(create(QString,QString,bool)));
# endif

  showDialog(dialog);
}


void ChannelsView::join(const QString &name)
{
  if (name.size() == 34)
    ChatNotify::start(Notify::OpenChannel, SimpleID::decode(name));
  else
    ChatClient::channels()->join(name);
}


void ChannelsView::create(const QString &id, const QString &name, bool _private)
{
  const QByteArray channelId = SimpleID::decode(id);
  if (SimpleID::typeOf(channelId) != SimpleID::ChannelId)
    return;

  m_channels[channelId] = _private;
  ChatClient::channels()->join(name);
}


void ChannelsView::contextMenu(QMenu *menu, const QWebHitTestResult &result)
{
  menu->addSeparator();

  const QUrl url = result.linkUrl();
  if (url.scheme() == LS("chat") && url.host() == LS("channel"))
    ChannelMenu::bind(menu, ChatUrls::channel(url), IChannelMenu::ChatViewScope);

  menu->addSeparator();

  if (!result.isContentEditable()) {
    menu->removeAction(pageAction(QWebPage::SelectAll));
    menu->addAction(pageAction(QWebPage::SelectAll));
  }
}


void ChannelsView::channel(const QByteArray &id)
{
  if (!m_channels.contains(id))
    return;

  if (m_channels.value(id)) {
    ChatClientLocker locker(ChatClient::io());
    ClientFeeds::post(id, INFO_FEED_SUDO_REQ,       true, Feed::NoOptions);
    ClientFeeds::post(id, INFO_FEED_VISIBILITY_REQ, -1,   Feed::NoOptions);
    ClientFeeds::put(id,  ACL_FEED_HEAD_MASK_REQ,   448,  Feed::Share | Feed::Broadcast);
  }

  m_channels.remove(id);
  TabWidget::pin(id);
}


void ChannelsView::notify(const Notify &notify)
{
  int type = notify.type();
  if (type == Notify::FeedData || type == Notify::FeedReply) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if (n.channel() != ChatClient::serverId())
      return;

    QVariantMap json = WebBridge::feed(n);
    if (json.isEmpty())
      return;

    emit feed(json);
  }
  else if (type == Notify::ServerRenamed) {
    emit serverRenamed(ChatClient::serverName());
  }
}


void ChannelsView::populateJavaScriptWindowObject()
{
  page()->mainFrame()->addToJavaScriptWindowObject(LS("SimpleChat"), WebBridge::i());
  page()->mainFrame()->addToJavaScriptWindowObject(LS("ChannelsView"), this);
}
