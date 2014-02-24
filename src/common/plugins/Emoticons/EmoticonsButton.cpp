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

#include <QEvent>
#include <QMenu>
#include <QWidgetAction>

#include "EmoticonsButton.h"
#include "sglobal.h"
#include "EmoticonsTabs.h"

EmoticonsButton::EmoticonsButton(Emoticons *emoticons, QWidget *parent)
  : QToolButton(parent)
  , m_emoticons(emoticons)
{
  m_menu = new QMenu(this);

  setAutoRaise(true);
  setIcon(QIcon(LS(":/images/Emoticons/edit.png")));
  setPopupMode(QToolButton::InstantPopup);
  setMenu(m_menu);
  setStyleSheet(LS("QToolButton::menu-indicator {image:none}"));

  retranslateUi();

  connect(m_menu, SIGNAL(aboutToHide()), SLOT(menuAboutToHide()));
  connect(m_menu, SIGNAL(aboutToShow()), SLOT(menuAboutToShow()));
}


void EmoticonsButton::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QToolButton::changeEvent(event);
}


void EmoticonsButton::menuAboutToHide()
{
  QList<QAction *> actions = m_menu->actions();
  foreach (QAction *action, actions)
    action->deleteLater();

  m_menu->clear();
}


void EmoticonsButton::menuAboutToShow()
{
  QWidgetAction *action = new QWidgetAction(this);
  action->setDefaultWidget(new EmoticonsTabs(m_emoticons, this));
  m_menu->addAction(action);
}


void EmoticonsButton::retranslateUi()
{
  setToolTip(tr("Emoticons"));
}


EmoticonsAction::EmoticonsAction(Emoticons *emoticons)
  : ToolBarActionCreator(1100, LS("emoticons"), WidgetType | AutoShow | AutoDelete)
  , m_emoticons(emoticons)
{
}


QWidget* EmoticonsAction::createWidget(QWidget *parent) const
{
  return new EmoticonsButton(m_emoticons, parent);
}


QIcon EmoticonsAction::icon() const
{
  return QIcon(LS(":/images/Emoticons/edit.png"));
}


QString EmoticonsAction::title() const
{
  return tr("Emoticons");
}
