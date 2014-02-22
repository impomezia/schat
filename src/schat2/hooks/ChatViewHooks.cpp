/* $Id: ChatViewHooks.cpp 3754 2013-07-14 19:50:00Z IMPOMEZIA $
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

#include <QWebHitTestResult>

#include "hooks/ChatViewHooks.h"
#include "ui/tabs/ChatView.h"

ChatViewHooks *ChatViewHooks::m_self = 0;

ChatViewHooks::ChatViewHooks(QObject *parent)
  : QObject(parent)
{
  if (!m_self)
    m_self = this;
  else
    add(this);
}


ChatViewHooks::~ChatViewHooks()
{
  if (m_self == this)
    m_self = 0;
}


bool ChatViewHooks::contextMenu(ChatView *view, QMenu *menu, const QWebHitTestResult &result)
{
  bool modified = false;
  if (m_self) {
    foreach (ChatViewHooks *hook, m_self->m_hooks) {
      if (hook->onContextMenu(view, menu, result) && !modified)
        modified = true;
    }
  }

  return modified;
}


bool ChatViewHooks::dragEnterEvent(ChatView *view, QDragEnterEvent *event)
{
  if (m_self) {
    foreach (ChatViewHooks *hook, m_self->m_hooks) {
      if (hook->onDragEnterEvent(view, event))
        return true;
    }
  }

  return false;
}


bool ChatViewHooks::dropEvent(ChatView *view, QDropEvent *event)
{
  if (m_self) {
    foreach (ChatViewHooks *hook, m_self->m_hooks) {
      if (hook->onDropEvent(view, event))
        return true;
    }
  }

  return false;
}


void ChatViewHooks::retranslate()
{
  if (m_self) {
    foreach (ChatViewHooks *hook, m_self->m_hooks)
      hook->onRetranslate();
  }
}


bool ChatViewHooks::onContextMenu(ChatView *view, QMenu *menu, const QWebHitTestResult &result)
{
  Q_UNUSED(view)
  Q_UNUSED(menu)
  Q_UNUSED(result)

  return false;
}


bool ChatViewHooks::onDragEnterEvent(ChatView *view, QDragEnterEvent *event)
{
  Q_UNUSED(view);
  Q_UNUSED(event);

  return false;
}


bool ChatViewHooks::onDropEvent(ChatView *view, QDropEvent *event)
{
  Q_UNUSED(view);
  Q_UNUSED(event);

  return false;
}


void ChatViewHooks::addImpl(ChatView *view)
{
  if (m_self != this)
    return;

  if (!m_views.contains(view))
    m_views.append(view);

  if (!view->id().isEmpty())
    m_map[view->id()] = view;

  foreach (ChatViewHooks *hook, m_hooks) {
    hook->addImpl(view);
  }

  emit addHook(view);
}


void ChatViewHooks::initImpl(ChatView *view)
{
  if (m_self != this)
    return;

  foreach (ChatViewHooks *hook, m_hooks) {
    hook->initImpl(view);
  }

  emit initHook(view);
}


/*!
 * Обработка завершения загрузки страницы.
 */
void ChatViewHooks::loadFinishedImpl(ChatView *view)
{
  if (m_self != this)
    return;

  foreach (ChatViewHooks *hook, m_hooks) {
    hook->loadFinishedImpl(view);
  }

  emit loadFinishedHook(view);
}


void ChatViewHooks::removeImpl(ChatView *view)
{
  if (m_self != this)
    return;

  foreach (ChatViewHooks *hook, m_hooks) {
    hook->removeImpl(view);
  }

  emit removeHook(view);
  m_views.removeAll(view);
  m_map.remove(view->id());
}
