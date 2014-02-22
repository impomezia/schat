/* $Id: HistoryChatView.cpp 3755 2013-07-14 23:11:47Z IMPOMEZIA $
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

#include <QMenu>
#include <QWebFrame>
#include <QWebElement>

#include "acl/AclValue.h"
#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientFeeds.h"
#include "client/SimpleClient.h"
#include "feeds/MessagesFeed.h"
#include "HistoryChatView.h"
#include "HistoryDB.h"
#include "HistoryPlugin_p.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/ChatView.h"

HistoryChatView::HistoryChatView(QObject *parent)
  : ChatViewHooks(parent)
  , m_remove(0)
{
  synced();

  connect(ChatClient::io(), SIGNAL(ready()), SLOT(ready()));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
  connect(ChatCore::settings(), SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));
  connect(ChatCore::settings(), SIGNAL(synced()), SLOT(synced()));
}


bool HistoryChatView::isAutoLoad(const QString &id) const
{
  if (!m_autoLoad && SimpleID::typeOf(SimpleID::decode(id)) == SimpleID::UserId)
    return false;

  return true;
}


bool HistoryChatView::onContextMenu(ChatView *view, QMenu *menu, const QWebHitTestResult &result)
{
  ChatId id(view->id());
  if (id.type() != ChatId::ChannelId && id.type() != ChatId::UserId)
    return false;

  const QWebElement block = result.enclosingBlockElement();
  if (!block.hasClass("blocks") || block.hasClass("removed"))
    return false;

  const QWebElement container = block.parent();
  const qint64 mdate          = container.attribute(LS("data-mdate")).toLongLong();

  if (!mdate)
    return false;

  id.init(container.attribute(LS("id")).toLatin1());
  id.setDate(mdate);
  if (id.type() != ChatId::MessageId)
    return false;

  const int permissions = this->permissions(HistoryDB::get(id));
  if (permissions == NoPermissions)
    return false;

  if (permissions & Remove) {
    QVariantList data;
    data << view->id() << (id.hasOid() ? ChatId::toBase32(id.oid().byteArray()) : id.toString());

    menu->insertAction(menu->actions().first(), removeAction(data));
  }
  return true;
}


void HistoryChatView::addImpl(ChatView *view)
{
  if (compatible(view->id()) && sync(view->id()))
    emit loading(SimpleID::encode(view->id()));
}


void HistoryChatView::initImpl(ChatView *view)
{
  view->addJS(LS("qrc:/js/History/days.js"));

  if (compatible(view->id())) {
    view->page()->mainFrame()->addToJavaScriptWindowObject(LS("HistoryView"), this);
    view->addJS(LS("qrc:/js/History/History.js"));
  }
}


void HistoryChatView::loadFinishedImpl(ChatView *view)
{
  view->addCSS(LS("qrc:/css/History/History.css"));
}


void HistoryChatView::onRetranslate()
{
  if (m_remove) m_remove->setText(tr("Remove message"));
}


void HistoryChatView::notify(const Notify &notify)
{
  if (notify.type() == Notify::FeedReply) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if (n.feed() == FEED_NAME_MESSAGES && n.path() == MESSAGES_FEED_LAST_KEY) {
      if (n.status() == Notice::NotModified) {
        const QList<QByteArray> messages = HistoryDB::last(n.channel());
        HistoryImpl::get(n.channel(), messages);
        emulateLast(n.channel(), messages);
      }
      else if (n.status() == Notice::OK && !n.json().contains(MESSAGES_FEED_BEFORE_KEY) && !n.json().contains(MESSAGES_FEED_EMULATED_KEY))
        HistoryDB::add(n.channel(), n.json().value(MESSAGES_FEED_MESSAGES_KEY).toStringList());
    }
  }
}


/*!
 * Запрос последних сообщений для всех открытых каналов.
 */
void HistoryChatView::ready()
{
  ChatClientLocker locker;

  foreach (ChatView *view, i()->views()) {
    const QByteArray &id = view->id();
    if (compatible(id) && sync(id, view->lastMessage())) {
      emit loading(SimpleID::encode(id));
    }
  }

  ClientFeeds::request(ChatClient::id(), FEED_METHOD_GET, MESSAGES_FEED_OFFLINE_REQ);
}


void HistoryChatView::remove()
{
  const QVariantList data = m_remove->data().toList();
  if (data.size() < 2)
    return;

  ClientFeeds::del(data.at(0).toByteArray(), FEED_NAME_MESSAGES + '/' + data.at(1).toString(), Feed::Share | Feed::ShareAll);
}


void HistoryChatView::settingsChanged(const QString &key, const QVariant &value)
{
  if (key == SETTINGS_HISTORY_AUTO_LOAD)
    m_autoLoad = value.toBool();
}


void HistoryChatView::synced()
{
  m_autoLoad = ChatCore::settings()->value(SETTINGS_HISTORY_AUTO_LOAD).toBool();
}


/*!
 * Возвращает \b true если идентификатор является идентификатором обычного канала или пользователя.
 */
bool HistoryChatView::compatible(const QByteArray &id) const
{
  const int type = SimpleID::typeOf(id);
  if (type == SimpleID::ChannelId || type == SimpleID::UserId)
    return true;

  return false;
}


/*!
 * Синхронизации списка последних сообщений в канале.
 *
 * \param id   Идентификатор канала.
 * \param date Дата последнего полученного сообщения, если равно 0 запрашиваются 20 последних сообщений.
 */
bool HistoryChatView::sync(const QByteArray &id, qint64 date)
{
  if (ChatClient::state() != ChatClient::Online) {
    HistoryImpl::getLocal(HistoryDB::last(id));
    return false;
  }

  if (date)
    return HistoryImpl::since(id, date);

  QVariantMap json;
  const QString tag = HistoryDB::tag(id);
  if (!tag.isEmpty())
    json[MESSAGES_FEED_TAG_KEY] = tag;

  if (!m_autoLoad && SimpleID::typeOf(id) == SimpleID::UserId)
    return false;

  return ClientFeeds::request(id, FEED_METHOD_GET, MESSAGES_FEED_LAST_REQ, json);
}


/*!
 * Определение доступных прав пользователя, для определения какие действия над сообщением возможны.
 */
int HistoryChatView::permissions(const MessageRecord &record) const
{
  if (!record.id)
    return NoPermissions;

  FeedPtr messages = ChatClient::server()->feed(FEED_NAME_MESSAGES, false);
  if (!messages)
    return NoPermissions;

  const QVariantMap data = messages->data();
  const int flags        = data.value(MESSAGES_FEED_EDITABLE_KEY, DefaultEditFlags).toInt();
  const bool timeout     = (qAbs(ChatClient::date() - record.date) / 1000) > data.value(MESSAGES_FEED_TIMEOUT_KEY, DefaultTimeOut).toInt();

  if (record.senderId == ChatClient::id() && (flags & SelfEdit) && !timeout)
    return Remove | Modify;

  if (ChatId(record.destId).type() != ChatId::ChannelId)
    return NoPermissions;

  const int acl = ClientFeeds::match(ChatClient::channels()->get(record.destId), ChatClient::channel());
  if ((acl & Acl::SpecialWrite) || (acl & Acl::Edit)) {
    int out = 0;
    if (flags & ModeratorRemove) out |= Remove;
    if (flags & ModeratorEdit)   out |= Modify;

    return out;
  }

  return NoPermissions;
}


QAction *HistoryChatView::removeAction(const QVariant &data)
{
  if (!m_remove) {
    m_remove = new QAction(SCHAT_ICON(Remove), tr("Remove message"), this);
    connect(m_remove, SIGNAL(triggered()), SLOT(remove()));
  }

  m_remove->setData(data);
  return m_remove;
}


/*!
 * Эмуляция ответа на запрос последних сообщений.
 *
 * \param channelId Идентификатор канала.
 * \param ids       Список идентификаторов сообщений.
 */
void HistoryChatView::emulateLast(const QByteArray &channelId, const QList<QByteArray> &ids)
{
  QVariantMap data;
  data[MESSAGES_FEED_COUNT_KEY]    = ids.size();
  data[MESSAGES_FEED_MESSAGES_KEY] = MessageNotice::encode(ids);
  data[MESSAGES_FEED_EMULATED_KEY] = true;

  FeedNotify *notify = new FeedNotify(Notify::FeedReply, channelId, MESSAGES_FEED_LAST_REQ, data);
  ChatNotify::start(notify);
}
