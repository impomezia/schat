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

#include <QMenu>
#include <QToolButton>
#include <QEvent>

#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/AbstractTab.h"
#include "ui/tabs/ServerTab.h"
#include "ui/TabsToolBar.h"
#include "ui/TabWidget.h"

TabsToolBar::TabsToolBar(QWidget *parent)
  : QToolBar(parent)
{
  setIconSize(QSize(16, 16));

  m_menu = new QMenu(this);

  m_button = new QToolButton(this);
  m_button->setIcon(SCHAT_ICON(MainTabMenu));
  m_button->setAutoRaise(true);
  m_button->setPopupMode(QToolButton::InstantPopup);
  m_button->setToolTip(tr("Menu"));
  m_button->setMenu(m_menu);
  m_button->setStyleSheet(LS("QToolButton::menu-indicator {image:none}"));

  m_talks = new QMenu(tr("Talks"), this);
  m_talks->setIcon(SCHAT_ICON(Channel));

  addWidget(m_button);

  connect(m_menu, SIGNAL(aboutToHide()), SLOT(hideMenu()));
  connect(m_menu, SIGNAL(aboutToShow()), SLOT(showMenu()));
}


void TabsToolBar::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QToolBar::changeEvent(event);
}


void TabsToolBar::hideMenu()
{
  m_menu->clear();
  m_talks->clear();
}


void TabsToolBar::showMenu()
{
  QList<QAction *> talks;
  TabWidget *tabs = TabWidget::i();
  AbstractTab *currentTab = tabs->widget(tabs->currentIndex());

  for (int i = 0; i < tabs->count(); ++i) {
    AbstractTab *tab = tabs->widget(i);
    if (tab->type() == LS("channel") || tab->type() == LS("talk"))
      talks.append(tab->action());

    tab->action()->setChecked(currentTab == tab);
  }

  if (!talks.isEmpty()) {
    m_talks->addActions(talks);
    m_menu->addMenu(m_talks);
  }

  QAction *separator = m_menu->addSeparator();
  m_menu->addAction(tabs->serverTab()->action());
  tabs->serverTab()->action()->setChecked(currentTab == tabs->serverTab());

  emit showMenu(m_menu, separator);
}


void TabsToolBar::retranslateUi()
{
  m_button->setToolTip(tr("Menu"));
  m_talks->setTitle(tr("Talks"));
}
