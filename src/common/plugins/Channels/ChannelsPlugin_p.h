/* $Id: ChannelsPlugin_p.h 3594 2013-03-24 19:33:28Z IMPOMEZIA $
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

#ifndef CHANNELSPLUGIN_P_H_
#define CHANNELSPLUGIN_P_H_

#include "Channel.h"
#include "plugins/ChatPlugin.h"

class ChannelsTr;
class ChatView;
class QAction;
class QMenu;

class ChannelsPluginImpl : public ChatPlugin
{
  Q_OBJECT

public:
  ChannelsPluginImpl(QObject *parent);
  ~ChannelsPluginImpl();
  static void ignore(const QByteArray &id, bool ignore = true);
  static void inviteTo(const QByteArray &userId, const QByteArray &channelId);
  static void setAcl(const QByteArray &userId, const QByteArray &channelId, int acl);
  static void show();

private slots:
  inline void list() { show(); }
  void channel(const QByteArray &id);
  void init(ChatView *view);
  void loadFinished(ChatView *view);
  void ready();
  void showMenu(QMenu *menu, QAction *separator);
  void start();

private:
  void getAcl(ClientChannel channel);

  ChannelsTr *m_tr; ///< Перевод.
  QAction *m_list;  ///< Пункт меню для показа списка каналов.
};

#endif /* CHANNELSPLUGIN_P_H_ */
