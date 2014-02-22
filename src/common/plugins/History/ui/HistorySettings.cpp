/* $Id: HistorySettings.cpp 3603 2013-03-27 15:19:21Z IMPOMEZIA $
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

#include <QCheckBox>
#include <QEvent>
#include <QVBoxLayout>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "ui/HistorySettings.h"

HistorySettings::HistorySettings(QWidget *parent)
  : QWidget(parent)
{
  m_autoLoad = new QCheckBox(this);
  m_autoLoad->setChecked(ChatCore::settings()->value(SETTINGS_HISTORY_AUTO_LOAD).toBool());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_autoLoad);
  layout->setMargin(0);

  retranslateUi();

  connect(m_autoLoad, SIGNAL(clicked(bool)), SLOT(autoLoad(bool)));
}


void HistorySettings::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void HistorySettings::autoLoad(bool checked)
{
  ChatCore::settings()->setValue(SETTINGS_HISTORY_AUTO_LOAD, checked);
}


void HistorySettings::retranslateUi()
{
  m_autoLoad->setText(tr("Automatically display last 20 private messages"));
}
