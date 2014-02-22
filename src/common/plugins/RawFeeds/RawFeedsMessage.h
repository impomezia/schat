/* $Id: RawFeedsMessage.h 3012 2012-08-17 21:53:31Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#ifndef RAWFEEDSMESSAGE_H_
#define RAWFEEDSMESSAGE_H_

#include "messages/Message.h"

class FeedNotice;

class RawFeedsMessage : public Message
{
public:
  RawFeedsMessage(const FeedNotice &packet);
  RawFeedsMessage(const QByteArray &tab, const QString &command, const QVariantMap &data);

private:
  void feed(const QString &name, int size);
  void headers(const QVariantMap &data);
};

#endif /* RAWFEEDSMESSAGE_H_ */
