/* $Id: ChannelsSettings.cpp 3574 2013-03-11 23:31:35Z IMPOMEZIA $
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

#include "ChannelsSettings.h"
#include "ChatCore.h"
#include "ChatSettings.h"
#include "sglobal.h"

ChannelsSettings::ChannelsSettings(QWidget *parent)
  : QWidget(parent)
{
  m_ignoring = new QCheckBox(this);
  m_ignoring->setChecked(ChatCore::settings()->value(SETTINGS_CHANNELS_IGNORING).toBool());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_ignoring);
  layout->setMargin(0);

  retranslateUi();

  connect(m_ignoring, SIGNAL(clicked(bool)), SLOT(ignoring(bool)));
}


void ChannelsSettings::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void ChannelsSettings::ignoring(bool checked)
{
  ChatCore::settings()->setValue(SETTINGS_CHANNELS_IGNORING, checked);
}


void ChannelsSettings::retranslateUi()
{
  m_ignoring->setText(tr("Enable ignoring users via context menu"));
}
