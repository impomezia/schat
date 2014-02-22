/* $Id: IdleSettings.cpp 3453 2013-02-01 18:36:49Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#include <QSpinBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "IdleSettings.h"
#include "sglobal.h"

IdleSettings::IdleSettings(QWidget *parent)
  : QWidget(parent)
{
  m_label = new QLabel(this);
  m_time = new QSpinBox(this);
  m_time->setRange(0, 1440);
  m_time->setValue(ChatCore::settings()->value(SETTINGS_AUTO_AWAY).toInt() / 60);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(m_label);
  layout->addWidget(m_time);
  layout->addStretch();
  layout->setMargin(0);

  retranslateUi();

  connect(m_time, SIGNAL(valueChanged(int)), SLOT(changed(int)));
}


void IdleSettings::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void IdleSettings::changed(int value)
{
  if (value > 0)
    value *= 60;

  ChatCore::settings()->setValue(SETTINGS_AUTO_AWAY, value);
}


void IdleSettings::retranslateUi()
{
  m_label->setText(tr("Away after"));
  m_time->setSuffix(LC(' ') + tr("min"));
}
