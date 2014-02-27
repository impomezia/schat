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

#include <QWebHitTestResult>

#include "hooks/ChatViewHooks.h"
#include "ui/tabs/ChatView.h"
#include "interfaces/IChatViewHook.h"

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
  Q_ASSERT(m_self && view && menu);

  if (!m_self)
    return false;

  bool modified = false;

  foreach (IChatViewHook *hook, m_self->m_hooks) {
    if (hook->contextMenu(view, menu, result) && !modified)
      modified = true;
  }

  return modified;
}


bool ChatViewHooks::dragEnterEvent(ChatView *view, QDragEnterEvent *event)
{
  Q_ASSERT(m_self && view && event);

  if (!m_self)
    return false;

  foreach (IChatViewHook *hook, m_self->m_hooks) {
    if (hook->dragEnterEvent(view, event))
      return true;
  }

  return false;
}


bool ChatViewHooks::dropEvent(ChatView *view, QDropEvent *event)
{
  Q_ASSERT(m_self && view && event);

  if (!m_self)
    return false;

  foreach (IChatViewHook *hook, m_self->m_hooks) {
    if (hook->dropEvent(view, event))
      return true;
  }

  return false;
}


void ChatViewHooks::add(ChatView *view)
{
  Q_ASSERT(m_self && view);

  if (!m_self)
    return;

  if (!m_self->m_views.contains(view))
    m_self->m_views.append(view);

  if (!view->id().isEmpty())
    m_self->m_map.insert(view->id(), view);

  foreach (IChatViewHook *hook, m_self->m_hooks) {
    hook->add(view);
  }

  emit m_self->addHook(view);
}


void ChatViewHooks::add(IChatViewHook *hook)
{
  Q_ASSERT(m_self && hook);

  if (m_self && hook && !m_self->m_hooks.contains(hook))
    m_self->m_hooks.append(hook);
}


void ChatViewHooks::init(ChatView *view)
{
  Q_ASSERT(m_self && view);

  if (!m_self)
    return;

  foreach (IChatViewHook *hook, m_self->m_hooks) {
    hook->init(view);
  }

  emit m_self->initHook(view);
}


void ChatViewHooks::loadFinished(ChatView *view)
{
  Q_ASSERT(m_self && view);

  if (!m_self)
    return;

  foreach (IChatViewHook *hook, m_self->m_hooks) {
    hook->loadFinished(view);
  }

  emit m_self->loadFinishedHook(view);
}


void ChatViewHooks::remove(ChatView *view)
{
  Q_ASSERT(m_self && view);

  if (!m_self)
    return;

  foreach (IChatViewHook *hook, m_self->m_hooks) {
    hook->remove(view);
  }

  emit m_self->removeHook(view);
  m_self->m_views.removeAll(view);
  m_self->m_map.remove(view->id());
}


void ChatViewHooks::remove(IChatViewHook *hook)
{
  Q_ASSERT(m_self && hook);

  if (m_self)
    m_self->m_hooks.removeAll(hook);
}


void ChatViewHooks::retranslate()
{
  Q_ASSERT(m_self);

  if (!m_self)
    return;

  foreach (IChatViewHook *hook, m_self->m_hooks)
    hook->retranslate();
}
