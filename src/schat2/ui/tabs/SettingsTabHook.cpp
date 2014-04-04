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

#include <QVBoxLayout>

#include "ChatCore.h"
#include "hooks/PluginsPage.h"
#include "hooks/SettingsTabImpl.h"
#include "ui/tabs/SettingsTabHook.h"

SettingsTabHook *SettingsTabHook::m_self = 0;

SettingsPage::SettingsPage(const QIcon &icon, const QString &id, QWidget *parent)
  : QWidget(parent)
  , m_settings(ChatCore::settings())
  , m_icon(icon)
  , m_id(id)
  , m_mainLayout(0)
{
}


SettingsPage::SettingsPage(QWidget *parent)
  : QWidget(parent)
  , m_settings(ChatCore::settings())
  , m_mainLayout(0)
{
}


void SettingsPage::setupLayout()
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addLayout(m_mainLayout);
  layout->addStretch();

  QMargins margins = layout->contentsMargins();
  margins.setBottom(0);

  layout->setContentsMargins(margins);
}


SettingsTabHook::SettingsTabHook(QObject *parent)
  : QObject(parent)
{
  m_self = this;

  add(new ProfilePageCreator());
  add(new NetworkPageCreator());
  add(new AlertsPageCreator());
  add(new PluginsPageCreator());
  add(new LocalePageCreator());
}


SettingsTabHook::~SettingsTabHook()
{
  qDeleteAll(m_pages);
}


void SettingsTabHook::add(SettingsPage *page)
{
  emit m_self->added(page->id(), page);
}


void SettingsTabHook::add(SettingsPageCreator *creator)
{
  m_self->add(creator->weight(), creator);
}


void SettingsTabHook::add(int weight, SettingsPageCreator *creator)
{
  if (m_pages.contains(weight))
    add(++weight, creator);
  else
    m_pages[weight] = creator;
}
