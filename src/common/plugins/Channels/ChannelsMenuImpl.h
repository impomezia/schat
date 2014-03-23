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

#ifndef CHANNELSMENUIMPL_H_
#define CHANNELSMENUIMPL_H_

#include "interfaces/IChannelMenu.h"

class ChannelsMenuImpl : public QObject, public IChannelMenu
{
  Q_OBJECT

public:
  ChannelsMenuImpl(QObject *parent = 0);
  ~ChannelsMenuImpl();

protected:
  bool trigger(QAction *action) Q_DECL_OVERRIDE;
  void bind(QMenu *menu, ClientChannel channel, Scope scope) Q_DECL_OVERRIDE;
  void cleanup() Q_DECL_OVERRIDE;

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
