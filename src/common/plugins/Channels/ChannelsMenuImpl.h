/* $Id: ChannelsMenuImpl.h 3643 2013-04-17 10:50:31Z IMPOMEZIA $
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

#ifndef CHANNELSMENUIMPL_H_
#define CHANNELSMENUIMPL_H_

#include "hooks/ChannelMenu.h"

class ChannelsMenuImpl : public Hooks::ChannelMenu
{
  Q_OBJECT

public:
  ChannelsMenuImpl(QObject *parent = 0);

protected:
  bool triggerImpl(QAction *action);
  void bindImpl(QMenu *menu, ClientChannel channel, Hooks::Scope scope);
  void cleanupImpl();

private slots:
  void invite(QAction *action);

private:
  bool canInviteTo(ClientChannel user, ClientChannel channel);
  void invite(QMenu *menu, ClientChannel user);
  void permissions(QMenu *menu, ClientChannel user);

  bool m_self;          ///< \b true если пользователь совершает действия над собой.
  QAction *m_advanced;  ///< Расширенная настройка прав доступа.
  QAction *m_forbidden; ///< Бан пользователя в канале.
  QAction *m_ignore;    ///< Действие для игнорирования пользователя.
  QAction *m_kick;      ///< Удаление пользователя из канала.
  QAction *m_ro;        ///< Права только для чтения.
  QAction *m_rw;        ///< Права на чтение и запись.
  QByteArray m_id;      ///< Идентификатор канала.
  QMenu *m_invite;      ///< Меню приглашения в канал.
  QMenu *m_permissions; ///< Меню управления правами пользователя в канале.
};

#endif /* CHANNELSMENUIMPL_H_ */
