/* $Id: ServerTab.h 3174 2012-10-15 15:02:13Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#ifndef SERVERTAB_H_
#define SERVERTAB_H_

#include "ui/tabs/AbstractTab.h"

class Alert;
class AuthBridge;
class ChatView;
class Notify;

class ServerTab : public AbstractTab
{
  Q_OBJECT

public:
  ServerTab(TabWidget *parent);
  bool bindMenu(QMenu *menu);
  inline ChatView *chatView() { return m_chatView; }
  void copy();

protected:
  void changeEvent(QEvent *event);

private slots:
  void alert(const Alert &alert);
  void clientStateChanged(int state);
  void notify(const Notify &notify);
  void online();
  void populateJavaScriptWindowObject();

private:
  void retranslateUi();

  AuthBridge *m_auth;   ///< Мост между C++ и JavaScript кодом для поддержки авторизации.
  ChatView *m_chatView; ///< Виджет отображающий текст чата.
};

#endif /* SERVERTAB_H_ */
