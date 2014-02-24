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
#include <QMouseEvent>
#include <QUrl>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatUrls.h"
#include "client/ChatClient.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientFeeds.h"
#include "client/SimpleClient.h"
#include "debugstream.h"
#include "feeds/FeedStrings.h"
#include "hooks/ChannelMenu.h"
#include "hooks/MessagesImpl.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/UserView.h"

UserItem::UserItem(ClientChannel user, ClientChannel channel)
  : QStandardItem()
  , m_bold(false)
  , m_italic(false)
  , m_self(false)
  , m_underline(false)
  , m_channel(channel)
  , m_user(user)
{
  m_self = ChatClient::id() == user->id();

  reload();
}


/*!
 * Обновление информации.
 */
bool UserItem::reload()
{
  setText(m_user->name());
  setIcon(ChatIcons::icon(m_user));

  const int acl = ClientFeeds::match(m_channel, m_user);
  if (acl != -1) {
    QFont font = this->font();

    if (m_user->status() != Status::Offline) {
      m_bold      = acl & Acl::Edit || acl & Acl::SpecialWrite;
      m_italic    = !(acl & Acl::Write) || Hooks::MessagesImpl::isIgnored(m_user);
      m_underline = acl & Acl::SpecialEdit;
    }
    else {
      m_bold      = false;
      m_italic    = true;
      m_underline = false;
    }

    font.setBold(m_bold);
    font.setItalic(m_italic);
    font.setUnderline(m_underline);
    setFont(font);
  }

  setForeground(color());
  setData(QString::number(weight()) + m_user->name().toLower());

  return true;
}


/*!
 * Вес для сортировки.
 */
int UserItem::weight() const
{
  if (m_self)
    return 0;

  else if (m_user->status() == Status::Offline)
    return 9;

  else if (m_underline)
    return 1;

  else if (m_bold)
    return 2;

  else if (m_italic)
    return 8;

  else if (m_user->gender().value() == Gender::Bot)
    return 4;

  else if (m_user->status() == Status::FreeForChat)
    return 5;

  return 7;
}


/*!
 * Возвращает необходимый цвет текста.
 */
QBrush UserItem::color() const
{
  if (m_user->status() == Status::Offline)
    return QColor(0x90a4b3);

  return QPalette().brush(QPalette::WindowText);
}


UserView::UserView(ClientChannel channel, QWidget *parent)
  : QListView(parent)
  , m_sortable(true)
  , m_channel(channel)
{
  setModel(&m_model);
  setFocusPolicy(Qt::TabFocus);
  setEditTriggers(QListView::NoEditTriggers);
  setSpacing(1);
  setFrameShape(QFrame::NoFrame);

  setTextElideMode(Qt::ElideMiddle);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setStyleSheet(LS("QScrollBar:vertical{background:#fff;width:8px}QScrollBar::handle:vertical{background:#ccc;border-radius:3px}QScrollBar::sub-line:vertical,QScrollBar::add-line:vertical{height:0}QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}"));

  QPalette p = palette();
  if (p.color(QPalette::Base) == Qt::white) {
    setAlternatingRowColors(true);
    p.setColor(QPalette::AlternateBase, QColor(247, 250, 255));
    setPalette(p);
  }

  m_model.setSortRole(Qt::UserRole + 1);

  connect(this, SIGNAL(doubleClicked(QModelIndex)), SLOT(addTab(QModelIndex)));
  connect(ChatClient::channels(), SIGNAL(channel(ChannelInfo)), SLOT(channel(ChannelInfo)));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
}


/*!
 * Добавление канала-пользователя в список.
 *
 * \param user Указатель на канал.
 */
bool UserView::add(ClientChannel user)
{
  if (!user)
    return false;

  if (m_channels.contains(user->id()))
    return reload(user->id());

  UserItem *item = new UserItem(user, m_channel);

  m_model.appendRow(item);
  m_channels[user->id()] = item;

  if (m_sortable)
    sort();

  return true;
}


/*!
 * Обновление информации о пользователе.
 */
bool UserView::reload(const QByteArray &id)
{
  UserItem *item = m_channels.value(id);
  if (!item)
    return false;

  item->reload();
  if (m_sortable)
    sort();

  return true;
}


/*!
 * Удаление пользователя из списка.
 *
 * \return \b false если id не найден или пользователь офлайн.
 */
bool UserView::remove(const QByteArray &id)
{
  UserItem *item = m_channels.value(id);

  if (!item)
    return false;

  const int status = item->user()->status().value();
  m_model.removeRow(m_model.indexFromItem(item).row());
  m_channels.remove(id);

  return status != Status::Offline;
}


void UserView::clear()
{
  m_sortable = true;
  m_model.clear();
  m_channels.clear();
}


void UserView::sort()
{
  SCHAT_DEBUG_STREAM(this << "sort()");

  m_sortable = true;
  m_model.sort(0);
}


void UserView::channel(const ChannelInfo &info)
{
  UserItem *item = m_channels.value(info.id());
  if (!item)
    return;

  item->reload();

  if (info.option() == ChannelInfo::Renamed)
    sort();
}


void UserView::contextMenuEvent(QContextMenuEvent *event)
{
  QModelIndex index = indexAt(event->pos());

  if (!index.isValid()) {
    QListView::contextMenuEvent(event);
    return;
  }

  UserItem *item = static_cast<UserItem *>(m_model.itemFromIndex(index));
  QMenu menu(this);
  Hooks::ChannelMenu::bind(&menu, item->user(), Hooks::UserViewScope);
  menu.exec(event->globalPos());
}


void UserView::mouseReleaseEvent(QMouseEvent *event)
{
  QModelIndex index = indexAt(event->pos());

  if (index.isValid() && event->modifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton) {
    UserItem *item = static_cast<UserItem *>(m_model.itemFromIndex(index));
    ChatUrls::open(ChatUrls::toUrl(item->user(), LS("insert")));
  }
  else if (event->button() == Qt::LeftButton && !index.isValid()) {
    setCurrentIndex(QModelIndex());
  }
  else
    QListView::mouseReleaseEvent(event);
}


/*!
 * Обработка дабл-клика по пользователю в списке,
 * высылается сигнал с идентификатором пользователя.
 */
void UserView::addTab(const QModelIndex &index)
{
  ChatUrls::open(ChatUrls::toUrl(static_cast<UserItem *>(m_model.itemFromIndex(index))->user(), LS("open")));
}


void UserView::notify(const Notify &notify)
{
  if (notify.type() == Notify::FeedData) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if (n.feed() == FEED_NAME_ACL && (n.channel() == m_channel->id() || n.channel() == ChatClient::id() || n.channel() == ChatClient::serverId())) {
      foreach (UserItem *item, m_channels)
        item->reload();

      sort();
    }
  }
}
