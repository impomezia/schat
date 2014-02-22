/* $Id: ChannelBaseTab.h 3670 2013-06-01 20:28:52Z IMPOMEZIA $
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

#ifndef CHANNELBASETAB_H_
#define CHANNELBASETAB_H_

#include "Channel.h"
#include "ui/tabs/AbstractTab.h"

class ChannelInfo;
class ChatView;
class FindWidget;
class Message;
class Notify;

/*!
 * Базовый класс для вкладок каналов.
 */
class SCHAT_CORE_EXPORT ChannelBaseTab : public AbstractTab
{
  Q_OBJECT

public:
  ChannelBaseTab(ClientChannel channel, const QString &type, TabWidget *parent);
  ~ChannelBaseTab();

  inline ChatView *chatView() const    { return m_chatView; }
  inline ClientChannel channel() const { return m_channel; }

  bool bindMenu(QMenu *menu);
  int alerts() const;
  virtual bool isActive() const { return true; }
  void add(const Message &message);
  void copy();
  void setOnline(bool online = true);
  void stopAlert();

private slots:
  void channel(const ChannelInfo &info);
  void countChanged(int total, int count, const QByteArray &channel);
  void find(const QString &text, bool forward = true);
  void hidden();
  void notify(const Notify &notify);
  void offline();
  void setup();

protected:
  ClientChannel c();
  QIcon channelIcon();
  QString page() const;
  virtual void reload();
  virtual void layoutFindWidget() = 0;

  bool m_joined;            ///< \b true если пользователь уже вошёл в канал.
  ChatView *m_chatView;     ///< Виджет отображающий текст чата.
  FindWidget *m_findWidget; ///< Виджета поиска текста в окне чата.
  QByteArray m_serverId;    ///< Идентификатор сервера.
  QByteArray m_userId;      ///< Идентификатор пользователя.

private:
  ClientChannel m_channel; ///< Канал.
};

#endif /* CHANNELBASETAB_H_ */
