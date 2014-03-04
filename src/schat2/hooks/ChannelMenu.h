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

#ifndef CHANNELMENU_H_
#define CHANNELMENU_H_

#include "interfaces/IChannelMenu.h"

class SCHAT_CORE_EXPORT ChannelMenu : public QObject
{
  Q_OBJECT

public:
  ChannelMenu(QObject *parent = 0);
  ~ChannelMenu();
  static void add(IChannelMenu *hook);
  static void bind(QMenu *menu, ClientChannel channel, IChannelMenu::Scope scope);
  static void remove(IChannelMenu *hook);

protected slots:
  void cleanup();
  void triggered(QAction *action);

protected:
  virtual bool triggerImpl(QAction *action);
  virtual void bindImpl(QMenu *menu, ClientChannel channel, IChannelMenu::Scope scope);
  virtual void cleanupImpl();

private:
  static ChannelMenu *m_self;   ///< Указатель на себя.
  QList<IChannelMenu*> m_hooks; ///< Хуки.
};

#endif /* CHANNELMENU_H_ */
