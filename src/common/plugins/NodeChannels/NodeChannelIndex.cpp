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

#include <QTimer>

#include "acl/AclValue.h"
#include "Ch.h"
#include "DateTime.h"
#include "feeds/AclFeed.h"
#include "feeds/ChannelFeed.h"
#include "feeds/FeedEvents.h"
#include "feeds/FeedsCore.h"
#include "feeds/InfoFeed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "NodeChannelIndex.h"
#include "sglobal.h"

ChannelIndexData::ChannelIndexData(ChatChannel channel)
  : count(0)
  , options(NoOptions)
  , visibility(0)
  , name(channel->name())
{
  FeedPtr feed = channel->feed(FEED_NAME_INFO, false);
  if (!feed)
    return;

  visibility = feed->data().value(INFO_FEED_VISIBILITY_KEY, 0).toInt();
  if (visibility < 0)
    return;

  if (channel->permanent())
    options |= Permanent;

  id    = channel->id();
  count = channel->channels().size();
  title = feed->data().value(INFO_FEED_TITLE_KEY).toMap().value(INFO_FEED_TEXT_KEY).toString();

  if (feed->data().value(INFO_FEED_PINNED_KEY, false).toBool())
    options |= Pinned;

  FeedPtr acl = channel->feed(FEED_NAME_ACL, false);
  if (acl && AclValue::match(acl.data(), 0) == 0)
    options |= Private;
}


bool ChannelIndexData::isValid() const
{
  if (options & Permanent)
    return true;

  return count;
}


bool ChannelIndexData::operator<(const ChannelIndexData &other) const
{
  if ((options & Pinned) && !(other.options & Pinned))
    return true;

  if ((other.options & Pinned) && !(options & Pinned))
    return false;

  if (other.count == count)
    return name.toLower() < other.name.toLower();

  return other.count < count;
}


QVariantList ChannelIndexData::toList() const
{
  QVariantList list;
  list += SimpleID::encode(id);
  list += name;
  list += count;
  list += title;
  list += options;
  return list;
}


NodeChannelIndex::NodeChannelIndex(QObject *parent)
  : QObject(parent)
  , m_updated(false)
  , m_date(0)
{
  connect(FeedEvents::i(), SIGNAL(notify(FeedEvent)), SLOT(notify(FeedEvent)));

  QTimer::singleShot(0, this, SLOT(build()));
}


/*!
 * Создание списка каналов.
 */
void NodeChannelIndex::build()
{
  const QList<ChatChannel> channels = this->channels();

  m_list.clear();
  foreach (ChatChannel channel, channels) {
    ChannelIndexData data(channel);
    if (data.isValid())
      m_list.append(data);
  }

  qSort(m_list);
  m_date    = DateTime::utc();
  m_updated = true;

  FeedsCore::put(FEED_NAME_STATS + LS("/rooms"), QVariant(), Feed::Broadcast);
  FeedsCore::put(FEED_NAME_LIST + LS("/channels"), QVariant(), Feed::Broadcast);
}


void NodeChannelIndex::notify(const FeedEvent &event)
{
  if (isDirty(event))
    reload();
}


bool NodeChannelIndex::isDirty(const FeedEvent &event) const
{
  if (event.status != Notice::OK || event.method == FEED_METHOD_GET || SimpleID::typeOf(event.channel) != SimpleID::ChannelId)
    return false;

  if (event.name == FEED_NAME_INFO)
    return true;

  if (event.name == FEED_NAME_USERS && event.path.size() == 34)
    return true;

  if (event.name == FEED_NAME_CHANNEL && event.path == CHANNEL_FEED_NAME_KEY)
    return true;

  if (event.name == FEED_NAME_ACL && event.path == ACL_FEED_HEAD_MASK_KEY)
    return true;

  return false;
}


/*!
 * Получение списка обычных каналов, в которых есть пользователи.
 */
QList<ChatChannel> NodeChannelIndex::channels() const
{
  QList<ChatChannel> out;
  QList<QByteArray> all = Ch::server()->channels().all();

  foreach (const QByteArray &id, all) {
    if (SimpleID::typeOf(id) == SimpleID::ChannelId) {
      ChatChannel channel = Ch::channel(id, SimpleID::ChannelId, false);
      if (channel)
        out.append(channel);
    }
  }

  return out;
}


void NodeChannelIndex::reload()
{
  if (!m_date) {
    build();
  }
  else if (m_updated) {
    QTimer::singleShot(0, this, SLOT(build()));
    m_updated = false;
  }
}
