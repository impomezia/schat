/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#ifndef GENERICCH_H_
#define GENERICCH_H_

#include "ChHook.h"

class GenericCh : public ChHook
{
  Q_OBJECT

public:
  GenericCh(QObject *parent = 0);
  void newChannel(ChatChannel channel, ChatChannel user = ChatChannel()) Q_DECL_OVERRIDE;
  void rename(ChatChannel channel) Q_DECL_OVERRIDE;
  void server(ChatChannel channel, bool created) Q_DECL_OVERRIDE;
  void sync(ChatChannel channel, ChatChannel user = ChatChannel()) Q_DECL_OVERRIDE;
  void userChannel(ChatChannel channel) Q_DECL_OVERRIDE;

private slots:
  void onPacket(const SJMPPacket &packet);
};

#endif /* GENERICCH_H_ */
