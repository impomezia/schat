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

#include <QPalette>

#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "hooks/MessagesImpl.h"
#include "ui/ChatIcons.h"
#include "UserItem.h"

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


QVariant UserItem::data(int role) const
{
  if (role == StatusRole)
    return m_user->status().value();

  return QStandardItem::data(role);
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
