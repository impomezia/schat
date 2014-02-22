/* $Id: ChatViewHooks.h 3754 2013-07-14 19:50:00Z IMPOMEZIA $
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

#ifndef CHATVIEWHOOKS_H_
#define CHATVIEWHOOKS_H_

#include <QObject>
#include <QMap>

#include "schat.h"

class ChatView;
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
  inline static void add(ChatView *view)             { m_self->addImpl(view); }
  inline static void add(ChatViewHooks *hook)        { if (!m_self->m_hooks.contains(hook)) m_self->m_hooks.append(hook); }
  inline static void init(ChatView *view)            { m_self->initImpl(view); }
  inline static void loadFinished(ChatView *view)    { m_self->loadFinishedImpl(view); }
  inline static void remove(ChatView *view)          { m_self->removeImpl(view); }
  inline static void remove(ChatViewHooks *hook)     { m_self->m_hooks.removeAll(hook); }

  static bool contextMenu(ChatView *view, QMenu *menu, const QWebHitTestResult &result);
  static bool dragEnterEvent(ChatView *view, QDragEnterEvent *event);
  static bool dropEvent(ChatView *view, QDropEvent *event);
  static void retranslate();

signals:
  void addHook(ChatView *view);
  void initHook(ChatView *view);
  void loadFinishedHook(ChatView *view);
  void removeHook(ChatView *view);

protected:
  virtual bool onContextMenu(ChatView *view, QMenu *menu, const QWebHitTestResult &result);
  virtual bool onDragEnterEvent(ChatView *view, QDragEnterEvent *event);
  virtual bool onDropEvent(ChatView *view, QDropEvent *event);
  virtual void addImpl(ChatView *view);
  virtual void initImpl(ChatView *view);
  virtual void loadFinishedImpl(ChatView *view);
  virtual void onRetranslate() {}
  virtual void removeImpl(ChatView *view);

private:
  QList<ChatView*> m_views;          ///< Список окон.
  QList<ChatViewHooks*> m_hooks;     ///< Хуки.
  QMap<QByteArray, ChatView*> m_map; ///< Таблица окон.
  static ChatViewHooks *m_self;      ///< Указатель на себя.
};

#endif /* CHATVIEWHOOKS_H_ */
