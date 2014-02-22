/* $Id: PluginsPage.cpp 3408 2013-01-16 12:40:01Z IMPOMEZIA $
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

#include <QLabel>
#include <QVBoxLayout>

#include "ChatCore.h"
#include "ChatPlugins.h"
#include "hooks/PluginsPage.h"
#include "plugins/ChatApi.h"
#include "plugins/PluginsView.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"

PluginsPage::PluginsPage(QWidget *parent)
  : SettingsPage(SCHAT_ICON(Plugin), LS("plugins"), parent)
  , m_widget(0)
{
  m_label = new QLabel(this);
  m_view = new PluginsView(this);
  m_pluginName = new QLabel(this);
  m_pluginName->hide();

  m_mainLayout = new QVBoxLayout(this);
  m_mainLayout->addWidget(m_label);
  m_mainLayout->addWidget(m_view);
  m_mainLayout->addWidget(m_pluginName);
  m_mainLayout->setStretch(0, 0);
  m_mainLayout->setStretch(1, 1);

  QMargins margins = m_mainLayout->contentsMargins();
  margins.setBottom(0);
  m_mainLayout->setContentsMargins(margins);

  connect(m_view, SIGNAL(settings(QString)), SLOT(settings(QString)));

  retranslateUi();
}


void PluginsPage::retranslateUi()
{
  m_name = tr("Plugins");
  m_label->setText(LS("<b>") + m_name + LS("</b>"));
}


void PluginsPage::settings(const QString &id)
{
  if (!m_plugin.isEmpty()) {
    if (m_plugin == id)
      return;

    m_pluginName->hide();
    QLayoutItem *child = m_mainLayout->takeAt(2);
    if (child)
      delete child;

    if (m_widget) {
      m_mainLayout->removeWidget(m_widget);
      m_widget->deleteLater();
      m_widget = 0;
    }

    m_plugin.clear();
  }

  PluginItem *plugin = ChatCore::plugins()->plugin(id);
  if (!plugin)
    return;

  ChatApi *api = qobject_cast<ChatApi*>(plugin->plugin());
  if (!api)
    return;

  m_widget = api->settings(this);
  if (!m_widget)
    return;

  m_plugin = id;
  m_pluginName->setText(LS("<b>") + plugin->header().value(CORE_API_NAME).toString() + LS("</b>"));
  m_pluginName->show();
  m_mainLayout->insertSpacing(2, 10);
  m_mainLayout->insertWidget(4, m_widget);
  m_view->scroll(id);
}


SettingsPage* PluginsPageCreator::page(QWidget *parent)
{
  return new PluginsPage(parent);
}
