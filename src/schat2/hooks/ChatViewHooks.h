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

#ifndef CHATVIEWHOOKS_H_
#define CHATVIEWHOOKS_H_

#include <QObject>
#include <QMap>

#include "schat.h"

class ChatView;
class IChatViewHook;
class QDragEnterEvent;
class QDropEvent;
class QMenu;
class QWebHitTestResult;

class SCHAT_CORE_EXPORT ChatViewHooks : public QObject
{
  Q_OBJECT

public:
  ChatViewHooks(QObject *parent = 0);
  ~ChatViewHooks();
  inline const QList<ChatView *>& views() const      { return m_views; }
  inline static ChatView* view(const QByteArray &id) { return m_self->m_map.value(id); }
  inline static ChatViewHooks *i()                   { return m_self; }

  static bool contextMenu(ChatView *view, QMenu *menu, const QWebHitTestResult &result);
  static bool dragEnterEvent(ChatView *view, QDragEnterEvent *event);
  static bool dropEvent(ChatView *view, QDropEvent *event);
  static void add(ChatView *view);
  static void add(IChatViewHook *hook);
  static void init(ChatView *view);
  static void loadFinished(ChatView *view);
  static void remove(ChatView *view);
  static void remove(IChatViewHook *hook);
  static void retranslate();

signals:
  void addHook(ChatView *view);
  void initHook(ChatView *view);
  void loadFinishedHook(ChatView *view);
  void removeHook(ChatView *view);

private:
  QList<ChatView*> m_views;          ///< Список окон.
  QList<IChatViewHook*> m_hooks;     ///< Хуки.
  QMap<QByteArray, ChatView*> m_map; ///< Таблица окон.
  static ChatViewHooks *m_self;      ///< Указатель на себя.
};

#endif /* CHATVIEWHOOKS_H_ */
