/* $Id: HistoryFeedHook.cpp 3730 2013-07-06 17:27:17Z IMPOMEZIA $
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

#include <QStringList>

#include "ChatNotify.h"
#include "feeds/FeedStrings.h"
#include "feeds/MessagesFeed.h"
#include "HistoryFeedHook.h"
#include "id/ChatId.h"

bool HistoryFeedHook::hook(const FeedNotify &notify, QVariantMap &out)
{
  if (notify.name() != MESSAGES_FEED_LAST_REQ && notify.name() != MESSAGES_FEED_SINCE_REQ)
    return false;

  QVariantMap data = out.value(FeedNotify::kData).toMap();
  if (data.isEmpty())
    return false;

  const QStringList messages = data.value(MESSAGES_FEED_MESSAGES_KEY).toStringList();
  if (messages.isEmpty())
    return false;

  QStringList list;
# if QT_VERSION >= 0x040700
  list.reserve(messages.size());
# endif

  ChatId id;

  foreach (const QString &message, messages) {
    id.init(message.toLatin1());
    if (id.hasOid())
      list.append(ChatId::toBase32(id.oid().byteArray()));
    else
      list.append(message);
  }

  data.insert(MESSAGES_FEED_MESSAGES_KEY, list);
  out.insert(FeedNotify::kData, data);

  return true;
}
