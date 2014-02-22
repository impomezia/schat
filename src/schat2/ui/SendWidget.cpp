/* $Id: SendWidget.cpp 3764 2013-08-12 23:01:53Z IMPOMEZIA $
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

#include <QContextMenuEvent>
#include <QEvent>
#include <QMenu>
#include <QToolBar>
#include <QVBoxLayout>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "hooks/SendButton.h"
#include "hooks/ToolBarActions.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/InputWidget.h"
#include "ui/SendWidget.h"
#include "ui/tabs/AbstractTab.h"
#include "ui/TabWidget.h"

SendWidget *SendWidget::m_self = 0;

SendWidget::SendWidget(QWidget *parent)
  : QWidget(parent)
{
  m_self = this;

  m_layout = SCHAT_OPTION(LS("ToolBarActions")).toStringList();

  m_toolBar = new QToolBar(this);
  m_toolBar->setIconSize(QSize(16, 16));

  m_input = new InputWidget(this);

  QVBoxLayout *mainLay = new QVBoxLayout(this);
  mainLay->addWidget(m_toolBar);
  mainLay->addWidget(m_input);
  mainLay->setMargin(0);
  mainLay->setSpacing(0);

  add(new TextEditAction(InputWidget::Bold), false);
  add(new TextEditAction(InputWidget::Italic), false);
  add(new TextEditAction(InputWidget::Underline), false);
  add(new TextEditAction(InputWidget::Strike), false);
  add(new ColorAction(), false);
  add(new StretchAction(), false);
  add(new SendAction(), false);

  updateStyleSheet();
  refill();

  connect(m_input, SIGNAL(send(QString)), SIGNAL(send(QString)));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
  connect(ChatCore::settings(), SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));
  connect(TabWidget::i(), SIGNAL(pageChanged(AbstractTab*)), SLOT(pageChanged(AbstractTab*)));
}


void SendWidget::add(ToolBarActionCreator *creator, bool refill)
{
  m_self->add(creator->weight(), creator, refill);
}


void SendWidget::setInputFocus()
{
  m_input->setFocus();
}


void SendWidget::insertHtml(const QString &text)
{
  m_input->insertHtml(text);
}


bool SendWidget::event(QEvent *event)
{
# if defined(Q_OS_WIN32)
  if (event->type() == QEvent::ApplicationPaletteChange) {
    updateStyleSheet();
  }
# endif

  return QWidget::event(event);
}


/*!
 * Контекстное меню.
 */
void SendWidget::contextMenuEvent(QContextMenuEvent *event)
{
  QWidget::contextMenuEvent(event);

  QAction *a = m_toolBar->actionAt(event->pos());
  if (!a)
    return;

  QString name = a->data().toString();
  if (name.isEmpty())
    return;

  ToolBarAction action = m_names.value(name);
  if (!action)
    return;

  QMenu menu(this);
  QStringList available = this->available();
  QAction *remove = 0;

  if (!available.isEmpty()) {
    QMenu *addMenu = menu.addMenu(SCHAT_ICON(Add), tr("Add"));

    foreach (const QString &name, available) {
      ToolBarAction action = m_names.value(name);
      if (action) {
        QAction *a = addMenu->addAction(action->icon(), action->title());
        a->setData(name);
      }
    }
  }

  if (!(action->flags() & ToolBarActionCreator::Permanent))
    remove = menu.addAction(SCHAT_ICON(Remove), tr("Remove"));

  if (menu.actions().isEmpty())
    return;

  QAction *result = menu.exec(event->globalPos());
  if (!result)
    return;

  // Удаление кнопки.
  if (remove == result) {
    this->remove(action);

    if (action->flags() & ToolBarActionCreator::AutoShow)
      m_layout.append(LC('-') + name);
    else
      m_layout.removeAll(name);

    ChatCore::settings()->setValue(LS("ToolBarActions"), m_layout);
  }
  // Добавление кнопки.
  else if (result->data().type() == QVariant::String) {
    ToolBarAction action = m_names.value(result->data().toString());
    if (action) {
      add(action);

      if (action->flags() & ToolBarActionCreator::AutoShow)
        m_layout.removeAll(LC('-') + action->name());
      else
        m_layout.append(action->name());

      ChatCore::settings()->setValue(LS("ToolBarActions"), m_layout);
    }
  }
}


void SendWidget::notify(const Notify &notify)
{
  if (notify.type() == Notify::InsertText)
    insertHtml(notify.data().toString());
  else if (notify.type() == Notify::SetSendFocus)
    m_input->setFocus();
}


void SendWidget::pageChanged(AbstractTab *tab)
{
  foreach (const ToolBarAction &action, m_actions) {
    if (action->action())
      action->action()->setVisible(action->isVisible(tab->type(), tab->id()));
  }

  m_input->reload(tab->id());
}


void SendWidget::settingsChanged(const QString &key, const QVariant &value)
{
  if (key == LS("ToolBarActions")) {
    m_layout = value.toStringList();
    refill();
  }
}


/*!
 * Определяет действие, которое находится, после действия с весом \p weight.
 */
QAction* SendWidget::before(int weight) const
{
  const QList<int> keys = m_actions.keys();
  const int index = keys.indexOf(weight);
  if (index == -1)
    return 0;

  for (int i = index; i < keys.size(); ++i) {
    QAction *action = m_actions.value(keys.at(i))->action();
    if (action)
      return action;
  }

  return 0;
}


QStringList SendWidget::available() const
{
  QStringList out;
  QMapIterator<int, ToolBarAction> i(m_actions);
  while (i.hasNext()) {
    i.next();
    if (!i.value()->action())
      out.append(i.value()->name());
  }

  return out;
}


void SendWidget::add(int weight, ToolBarActionCreator *creator, bool refill)
{
  if (m_actions.contains(weight)) {
    add(++weight, creator);
    return;
  }

  ToolBarAction action = ToolBarAction(creator);
  m_actions[weight] = action;
  m_names[action->name()] = action;

  if (refill)
    this->refill();
}


/*!
 * Добавления действия на панель инструментов.
 */
void SendWidget::add(ToolBarAction action)
{
  if (!action)
    return;

  if (action->action())
    return;

  QAction *qa = 0;
  if (action->flags() & ToolBarActionCreator::ActionType) {
    qa = action->createAction(this);
    m_toolBar->insertAction(before(action->weight()), qa);
  }
  else if (action->flags() & ToolBarActionCreator::WidgetType)
    qa = m_toolBar->insertWidget(before(action->weight()), action->createWidget(this));

  if (qa)
    qa->setData(action->name());

  action->setAction(qa);
}


void SendWidget::refill()
{
  QMapIterator<int, ToolBarAction> i(m_actions);
  while (i.hasNext()) {
    i.next();
    if (i.value()->flags() & ToolBarActionCreator::AutoShow) {
      m_layout.contains(LC('-') + i.value()->name()) ? remove(i.value()) : add(i.value());
    }
    else {
      !m_layout.contains(i.value()->name()) ? remove(i.value()) : add(i.value());
    }
  }
}


void SendWidget::remove(ToolBarAction action)
{
  if (!action)
    return;

  if (!action->action())
    return;

  m_toolBar->removeAction(action->action());
  if (action->flags() & ToolBarActionCreator::AutoDelete)
    action->action()->deleteLater();

  action->setAction(0);
}


void SendWidget::updateStyleSheet()
{
# if !defined(Q_OS_MAC)
#  if defined(Q_OS_WIN32)
    m_toolBar->setStyleSheet(QString("QToolBar { background-color: %1; margin:0px; border:0px; }").arg(palette().color(QPalette::Window).name()));
#  else
    m_toolBar->setStyleSheet("QToolBar { margin:0px; border:0px; }");
#  endif
# endif
}
