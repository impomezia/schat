/* $Id: HistoryPlugin.cpp 3755 2013-07-14 23:11:47Z IMPOMEZIA $
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

#include <QApplication>
#include <QTime>
#include <QTimer>
#include <QtPlugin>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "client/ClientMessages.h"
#include "feeds/MessagesFeed.h"
#include "HistoryButton.h"
#include "HistoryChatView.h"
#include "HistoryDB.h"
#include "HistoryFeedHook.h"
#include "HistoryMessages.h"
#include "HistoryPlugin.h"
#include "HistoryPlugin_p.h"
#include "net/Protocol.h"
#include "net/SimpleID.h"
#include "NetworkManager.h"
#include "sglobal.h"
#include "Tr.h"
#include "Translation.h"
#include "ui/HistorySettings.h"
#include "ui/SendWidget.h"
#include "ui/tabs/PrivateTab.h"
#include "WebBridge.h"

class HistoryPluginTr : public Tr
{
  Q_DECLARE_TR_FUNCTIONS(ProfilePluginTr)

public:
  HistoryPluginTr() : Tr() { m_prefix = LS("history_"); }

protected:
  QString valueImpl(const QString &key) const
  {
    if (key == LS("history_loading"))   return tr("Loading messages");
    else if (key == LS("history_more")) return tr("More messages");
    return QString();
  }
};


HistoryImpl::HistoryImpl(QObject *parent)
  : ChatPlugin(parent)
{
  m_tr = new HistoryPluginTr();
  new HistoryDB(this);
  m_chatView = new HistoryChatView(this);
  new HistoryMessages(this);

  open();
  connect(ChatClient::i(), SIGNAL(online()), SLOT(open()));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));

  ChatCore::translation()->addOther(LS("history"));
  ChatCore::settings()->setDefault(SETTINGS_HISTORY_AUTO_LOAD, true);

  connect(ChatClient::i(), SIGNAL(ready()), SLOT(ready()));

  QTimer::singleShot(0, this, SLOT(start()));
}


HistoryImpl::~HistoryImpl()
{
  delete m_tr;
}


bool HistoryImpl::fetch(const QByteArray &id, const QList<QByteArray> &messages)
{
  QVariantMap data;
  data.insert(MESSAGES_FEED_MESSAGES_KEY, MessageNotice::encode(messages));
  data.insert(MESSAGES_FEED_V_KEY,        Protocol::V4_1);
  return ClientFeeds::request(id, FEED_METHOD_GET, MESSAGES_FEED_FETCH_REQ, data);
}


/*!
 * Загрузка сообщений по идентификаторам.
 */
bool HistoryImpl::get(const QByteArray &id, const QList<QByteArray> &messages)
{
  if (messages.isEmpty())
    return false;

  const QList<QByteArray> required = getLocal(messages);
  if (required.isEmpty() || ChatClient::state() != ChatClient::Online)
    return false;

  return fetch(id, required);
}


bool HistoryImpl::since(const QByteArray &id, qint64 date)
{
  QVariantMap data;
  data[MESSAGES_FEED_DATE_KEY] = date;
  return ClientFeeds::request(id, FEED_METHOD_GET, MESSAGES_FEED_SINCE_REQ, data);
}


/*!
 * Получение локальных сообщений, для сообщений найденных
 * в кэше эмулируется их получение для уведомления остальных
 * компонентов чата о новом сообщении.
 *
 * \param messages Список необходимых сообщений.
 *
 * \return Список сообщений не найденных в кэше.
 */
QList<QByteArray> HistoryImpl::getLocal(const QList<QByteArray> &messages)
{
  QList<QByteArray> out;
  for (int i = 0; i < messages.size(); ++i) {
    const MessageRecord record = HistoryDB::get(messages.at(i));
    if (!record.id) {
      out.append(messages.at(i));
      continue;
    }

    ChatClient::messages()->insert(new MessageNotice(record, /*parse=*/true));
  }

  return out;
}


void HistoryImpl::notify(const Notify &notify)
{
  if (notify.type() == Notify::FeedReply) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if ((n.name() == MESSAGES_FEED_LAST_REQ || n.name() == MESSAGES_FEED_SINCE_REQ) && !n.json().value(MESSAGES_FEED_EMULATED_KEY).toBool())
      get(n.channel(), MessageNotice::decode(n.json().value(MESSAGES_FEED_MESSAGES_KEY).toStringList()));
  }
  else if (notify.type() == Notify::ClearCache) {
    HistoryDB::clear();
  }
}


void HistoryImpl::open()
{
  const QByteArray id = ChatClient::serverId();
  if (!id.isEmpty())
    HistoryDB::open(id, ChatCore::networks()->root(SimpleID::encode(id)));
}


void HistoryImpl::ready()
{
  ClientFeeds::request(ChatClient::server(), FEED_METHOD_GET, FEED_NAME_MESSAGES);
}


void HistoryImpl::start()
{
  WebBridge::add(new HistoryFeedHook());
  SendWidget::add(new HistoryAction());
}


ChatPlugin *HistoryPlugin::create()
{
  m_plugin = new HistoryImpl(this);
  return m_plugin;
}


QWidget *HistoryPlugin::settings(QWidget *parent)
{
  return new HistorySettings(parent);
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(History, HistoryPlugin);
#endif
