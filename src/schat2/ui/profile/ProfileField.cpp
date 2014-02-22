/* $Id: ProfileField.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QLabel>
#include <QTimer>

#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "Profile.h"
#include "sglobal.h"
#include "ui/profile/ProfileField.h"

ProfileField::ProfileField(const QString &field, QWidget *parent)
  : QWidget(parent)
  , m_field(field)
{
  m_label = new QLabel(Profile::translate(field) + LC(':'), this);

  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));

  QTimer::singleShot(0, this, SLOT(reload()));
}


bool ProfileField::isMatch(const Notify &notify)
{
  int type = notify.type();
  if (type != Notify::FeedReply && type != Notify::FeedData)
    return false;

  const FeedNotify &n = static_cast<const FeedNotify &>(notify);
  return n.match(ChatClient::id(), FEED_NAME_PROFILE);
}


void ProfileField::retranslateUi()
{
  m_label->setText(Profile::translate(m_field) + LC(':'));
}


void ProfileField::reload()
{
  FeedPtr feed = ChatClient::channel()->feed(FEED_NAME_PROFILE, false);
  if (!feed)
    return;

  QVariant data = feed->data().value(m_field);
  if (data.isNull())
    return;

  setData(data);
}


void ProfileField::notify(const Notify &notify)
{
  if (!isMatch(notify))
    return;

  if (notify.type() == Notify::FeedReply) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if (!n.json().contains(m_field))
      return;
  }

  reload();
}


bool ProfileField::apply(const QVariant &value)
{
  FeedPtr feed = ChatClient::channel()->feed(FEED_NAME_PROFILE, false);
  if (!feed)
    return false;

  if (feed->data().value(m_field) == value)
    return false;

  return ClientFeeds::post(ChatClient::id(), FEED_NAME_PROFILE + LC('/') + m_field, value, Feed::Echo | Feed::Share | Feed::Broadcast);
}


void ProfileField::setData(const QVariant &)
{
}
