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
  , m_settings(ChatCore::settings())
  , m_fullscreen(0)
{
  m_enable = new QCheckBox(this);
  m_enable->setChecked(m_settings->value(ChatSettings::kAlertsPopup).toBool());

  QFont font = m_enable->font();
  font.setBold(true);
  m_enable->setFont(font);

  m_timeLabel = new QLabel(this);
  m_timeBox = new QSpinBox(this);
  m_timeBox->setRange(0, 86400);
  m_timeBox->setValue(m_settings->value(PopupPluginImpl::kTimeout).toInt());

  m_timeHintLabel = new QLabel(this);
  m_timeHintLabel->setEnabled(false);

# ifdef Q_OS_WIN
  m_fullscreen = new QCheckBox(this);
  m_fullscreen->setChecked(!m_settings->value(PopupPluginImpl::kFullscreen).toBool());
# endif

  if (!m_enable->isChecked()) {
    m_timeLabel->hide();
    m_timeBox->hide();
    m_timeHintLabel->hide();

    if (m_fullscreen)
      m_fullscreen->hide();
  }

  QGridLayout *mainLay = new QGridLayout(this);
  mainLay->addWidget(m_enable, 0, 0, 1, 3);
  mainLay->addWidget(m_timeLabel, 1, 0);
  mainLay->addWidget(m_timeBox, 1, 1);
  mainLay->addWidget(m_timeHintLabel, 1, 2);
  mainLay->setColumnStretch(2, 1);
  mainLay->setContentsMargins(10, 16, 0, 0);

  if (m_fullscreen) {
    mainLay->addWidget(m_fullscreen, 2, 0, 1, 3);
    connect(m_fullscreen, SIGNAL(clicked()), SLOT(save()));
  }

  retranslateUi();

  connect(m_enable, SIGNAL(clicked(bool)), SLOT(enable(bool)));
  connect(m_timeBox, SIGNAL(valueChanged(int)), SLOT(save()));
  connect(m_settings, SIGNAL(changed(QString,QVariant)), SLOT(onSettingsChanged(QString,QVariant)));
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
  m_settings->setValue(ChatSettings::kAlertsPopup, enable);

  if (m_fullscreen)
    m_fullscreen->setVisible(enable);
}


void PopupSettings::onSettingsChanged(const QString &key, const QVariant &value)
{
  if (key == ChatSettings::kAlertsPopup)
    m_enable->setChecked(value.toBool());
  else if (key == PopupPluginImpl::kTimeout)
    m_timeBox->setValue(value.toInt());
  else if (key == PopupPluginImpl::kFullscreen && m_fullscreen)
    m_fullscreen->setChecked(!value.toBool());
}


void PopupSettings::save()
{
  if (m_fullscreen)
    m_settings->setValue(PopupPluginImpl::kFullscreen, !m_fullscreen->isChecked());

  m_settings->setValue(PopupPluginImpl::kTimeout, m_timeBox->value());
}


void PopupSettings::retranslateUi()
{
  m_enable->setText(tr("Popup windows"));
  m_timeLabel->setText(tr("Time to display popup window"));
  m_timeBox->setSuffix(tr(" sec"));
  m_timeHintLabel->setText(tr("0 - infinitely"));

  if (m_fullscreen)
    m_fullscreen->setText(tr("Don't show popups when active fullscreen application"));
}
