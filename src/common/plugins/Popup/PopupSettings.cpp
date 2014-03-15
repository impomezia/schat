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

#include <QCheckBox>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "PopupPlugin_p.h"
#include "PopupSettings.h"
#include "sglobal.h"

PopupSettings::PopupSettings(QWidget *parent)
  : QWidget(parent)
{
  m_enable = new QCheckBox(this);
  m_enable->setChecked(ChatCore::settings()->value(ChatSettings::kAlertsPopup).toBool());

  QFont font = m_enable->font();
  font.setBold(true);
  m_enable->setFont(font);

  m_timeLabel = new QLabel(this);
  m_timeBox = new QSpinBox(this);
  m_timeBox->setRange(0, 86400);
  m_timeBox->setValue(ChatCore::settings()->value(PopupPluginImpl::kTimeout).toInt());

  m_timeHintLabel = new QLabel(this);
  m_timeHintLabel->setEnabled(false);

  if (!m_enable->isChecked()) {
    m_timeLabel->hide();
    m_timeBox->hide();
    m_timeHintLabel->hide();
  }

  QGridLayout *mainLay = new QGridLayout(this);
  mainLay->addWidget(m_enable, 0, 0, 1, 3);
  mainLay->addWidget(m_timeLabel, 1, 0);
  mainLay->addWidget(m_timeBox, 1, 1);
  mainLay->addWidget(m_timeHintLabel, 1, 2);
  mainLay->setColumnStretch(2, 1);
  mainLay->setContentsMargins(10, 16, 0, 0);

  retranslateUi();

  connect(m_enable, SIGNAL(clicked(bool)),SLOT(enable(bool)));
  connect(m_timeBox, SIGNAL(valueChanged(int)),SLOT(timeChanged(int)));
}


void PopupSettings::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void PopupSettings::enable(bool enable)
{
  m_timeLabel->setVisible(enable);
  m_timeBox->setVisible(enable);
  m_timeHintLabel->setVisible(enable);
  ChatCore::settings()->setValue(ChatSettings::kAlertsPopup, enable);
}


void PopupSettings::settingsChanged(const QString &key, const QVariant &value)
{
  if (key == ChatSettings::kAlertsPopup)
    m_enable->setChecked(value.toBool());
  else if (key == PopupPluginImpl::kTimeout)
    m_timeBox->setValue(value.toInt());
}


void PopupSettings::timeChanged(int time)
{
  ChatCore::settings()->setValue(PopupPluginImpl::kTimeout, time);
}


void PopupSettings::retranslateUi()
{
  m_enable->setText(tr("Popup windows"));
  m_timeLabel->setText(tr("Time to display popup window"));
  m_timeBox->setSuffix(tr(" sec"));
  m_timeHintLabel->setText(tr("0 - infinitely"));
}
