/* $Id: SendFileChatView.h 3682 2013-06-09 02:01:12Z IMPOMEZIA $
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

#ifndef SENDFILECHATVIEW_H_
#define SENDFILECHATVIEW_H_

#include <QUrl>

#include "hooks/ChatViewHooks.h"

class SendFilePluginImpl;

class SendFileChatView : public ChatViewHooks
{
  Q_OBJECT

public:
  SendFileChatView(SendFilePluginImpl *plugin);

protected:
  bool onDragEnterEvent(ChatView *view, QDragEnterEvent *event);
  bool onDropEvent(ChatView *view, QDropEvent *event);
  void initImpl(ChatView *view);
  void loadFinishedImpl(ChatView *view);

private:
  QStringList getFiles(const QList<QUrl> &urls) const;

  SendFilePluginImpl *m_plugin;
};

#endif /* SENDFILECHATVIEW_H_ */
