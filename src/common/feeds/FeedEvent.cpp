/* $Id: FeedEvent.cpp 3417 2013-01-18 09:06:25Z IMPOMEZIA $
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

#include "Channel.h"
#include "feeds/FeedEvent.h"

FeedEvent::FeedEvent()
  : status(0)
  , date(0)
  , diffTo(0)
  , socket(0)
{
}


FeedEvent::FeedEvent(const QByteArray &channel, const QByteArray &sender, const QString &method)
  : status(0)
  , channel(channel)
  , sender(sender)
  , date(0)
  , diffTo(0)
  , method(method)
  , socket(0)
{
}


bool FeedEvent::isValid() const
{
  if (!status || !date)
    return false;

  if (name.isEmpty() || method.isEmpty())
    return false;

  if (Channel::isCompatibleId(channel))
    return false;

  return true;
}
