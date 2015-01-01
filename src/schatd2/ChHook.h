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

#ifndef CHHOOK_H_
#define CHHOOK_H_

#include <QObject>

#include "ServerChannel.h"

class SCHAT_EXPORT ChHook : public QObject
{
  Q_OBJECT

public:
  ChHook(QObject *parent = 0);
  ~ChHook();

  virtual void add(ChatChannel channel);
  virtual void load();
  virtual void load(ChatChannel channel);
  virtual void newChannel(ChatChannel channel, ChatChannel user = ChatChannel());
  virtual void remove(ChatChannel channel);
  virtual void rename(ChatChannel channel);
  virtual void server(ChatChannel channel, bool created);
  virtual void sync(ChatChannel channel, ChatChannel user = ChatChannel());
  virtual void userChannel(ChatChannel channel);
};

#endif /* CHHOOK_H_ */
