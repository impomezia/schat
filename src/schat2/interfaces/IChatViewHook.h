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

#ifndef ICHATVIEWHOOK_H_
#define ICHATVIEWHOOK_H_

#include "schat.h"

class ChatView;
class QMenu;
class QWebHitTestResult;
class QDragEnterEvent;
class QDropEvent;

class IChatViewHook
{
public:
  virtual ~IChatViewHook() {}

  virtual bool contextMenu(ChatView *, QMenu * /*menu*/, const QWebHitTestResult & /*result*/) { return false; }
  virtual bool dragEnterEvent(ChatView *, QDragEnterEvent * /*event*/)                         { return false; }
  virtual bool dropEvent(ChatView *, QDropEvent * /*event*/)                                   { return false; }
  virtual void add(ChatView *)                                                                 {}
  virtual void init(ChatView *)                                                                {}
  virtual void loadFinished(ChatView *)                                                        {}
  virtual void remove(ChatView *)                                                              {}
  virtual void retranslate()                                                                   {}
};

#endif // ICHATVIEWHOOK_H_
