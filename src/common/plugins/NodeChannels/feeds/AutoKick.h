/* $Id: AutoKick.h 3643 2013-04-17 10:50:31Z IMPOMEZIA $
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

#ifndef AUTOKICK_H_
#define AUTOKICK_H_

#include <QObject>

#include "ServerChannel.h"

class FeedEvent;

class AutoKick : public QObject
{
  Q_OBJECT

public:
  AutoKick(QObject *parent = 0);

private slots:
  void notify(const FeedEvent &event);

private:
  void dump(ChatChannel channel) const;
  void kick(ChatChannel channel, const QByteArray &userId);
  void kickAll(ChatChannel channel);
};

#endif /* AUTOKICK_H_ */
