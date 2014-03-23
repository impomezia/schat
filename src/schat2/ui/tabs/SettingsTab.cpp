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

#include <QGridLayout>
#include <QListWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QStackedWidget>

#include "ChatSettings.h"
#include "ChatUrls.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/SettingsTab.h"
#include "ui/tabs/SettingsTabHook.h"

SettingsTab::SettingsTab(const QUrl &url, TabWidget *parent)
  : AbstractTab(SETTINGS_TAB, LS(SETTINGS_TAB), parent)
{
  m_contents = new QListWidget(this);
  m_contents->setSpacing(1);
  m_contents->setFrameShape(QFrame::NoFrame);
  m_pages = new QStackedWidget(this);

  QGridLayout *mainLay = new QGridLayout(this);
  mainLay->addWidget(m_contents, 0, 0);
  mainLay->addWidget(m_pages, 0, 1);
  mainLay->setMargin(0);
  mainLay->setSpacing(0);
  mainLay->setColumnStretch(0, 1);
  mainLay->setColumnStretch(1, 4);

  QMapIterator<int, SettingsPageCreator *> i(SettingsTabHook::i()->pages());
  while (i.hasNext()) {
    i.next();
    addPage(i.value()->page(this));
  }

  m_contents->setCurrentRow(0);
  setIcon(SCHAT_ICON(Gear));
  setText(tr("Preferences"));

  connect(m_contents, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(pageChanged(QListWidgetItem*,QListWidgetItem*)));

  openUrl(url);
}


void SettingsTab::addPage(SettingsPage *page)
{
  if (!page)
    return;

  if (m_ids.contains(page->id()))
    return;

  QListWidgetItem *item = new QListWidgetItem(page->icon(), page->name(), m_contents);
  m_items.append(item);
  m_ids.append(page->id());

  QScrollArea *scroll = new QScrollArea(this);
  scroll->setWidget(page);
  scroll->setWidgetResizable(true);
  scroll->setFrameShape(QFrame::NoFrame);
  m_pages->addWidget(scroll);

  SettingsTabHook::add(page);
}


void SettingsTab::openUrl(const QUrl &url)
{
  if (url.isEmpty() || url.scheme() != LS("chat") || url.host() != LS("settings"))
    return;

  QStringList path = ChatUrls::path(url);
  if (path.isEmpty())
    return;

  int page = m_ids.indexOf(path.at(0));
  if (page != -1)
    m_contents->setCurrentRow(page);
}


void SettingsTab::showEvent(QShowEvent *event)
{
  while (m_contents->horizontalScrollBar()->isVisible())
    m_contents->setMinimumWidth(m_contents->width() + 16);

  AbstractTab::showEvent(event);
}


void SettingsTab::pageChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
  if (!current)
    current = previous;

  m_pages->setCurrentIndex(m_contents->row(current));
}


void SettingsTab::retranslateUi()
{
  for (int i = 0; i < m_ids.size(); ++i) {
    SettingsPage *page = static_cast<SettingsPage *>(static_cast<QScrollArea *>(m_pages->widget(i))->widget());
    page->retranslateUi();
    m_contents->item(i)->setText(page->name());
  }

  setText(tr("Preferences"));
}


AbstractTab *SettingsTabCreator::create(const QVariant &data, TabWidget *parent) const
{
  return new SettingsTab(data.toUrl(), parent);
}


void SettingsTabCreator::reload(AbstractTab *tab, const QVariant &data) const
{
  SettingsTab *settings = qobject_cast<SettingsTab*>(tab);
  if (settings)
    settings->openUrl(data.toUrl());
}
