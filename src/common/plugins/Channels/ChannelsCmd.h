/* $Id: ChannelsCmd.h 3586 2013-03-22 12:36:44Z IMPOMEZIA $
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

#ifndef CHANNELSCMD_H_
#define CHANNELSCMD_H_

#include "hooks/MessagesHook.h"

class Alert;

class ChannelsCmd : public MessagesHook
{
  Q_OBJECT

public:
  ChannelsCmd(QObject *parent = 0);
  bool command(const QByteArray &dest, const ClientCmd &cmd);
  int read(MessagePacket packet);

private:
  QString inviteText(const QByteArray &tab, const QByteArray &channelId, const QString &name) const;
  void popup(Alert &alert, const QString &text) const;
};

#endif /* CHANNELSCMD_H_ */
