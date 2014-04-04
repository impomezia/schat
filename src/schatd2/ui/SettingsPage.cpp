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

#include <QApplication>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "Settings.h"
#include "SettingsPage.h"
#include "sglobal.h"


const QString SettingsPage::kApp = LS("Simple Chat Daemon");


SettingsPage::SettingsPage(Settings *settings, QWidget *parent)
  : QWidget(parent)
  , m_settings(settings)
{
  m_autoRun = new QCheckBox(this);
  m_autoRun->setChecked(isAutoRun());

  m_consoleBtn = new QCheckBox();
  m_consoleBtn->setChecked(m_settings->value(LS("Plugins/NodeConsole")).toBool());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_autoRun);
  layout->addWidget(m_consoleBtn);
  layout->addStretch();

  connect(m_autoRun, SIGNAL(clicked(bool)), SLOT(onAutoRunClicked(bool)));
  connect(m_consoleBtn, SIGNAL(clicked()), SLOT(save()));

  retranslateUi();
}


void SettingsPage::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void SettingsPage::onAutoRunClicked(bool checked)
{
  QSettings reg(LS("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
  if (checked)
    reg.setValue(kApp, QDir::toNativeSeparators(QApplication::applicationFilePath()) + LS(" -hide"));
  else
    reg.remove(kApp);
}


void SettingsPage::save()
{
  m_settings->setValue(LS("Plugins/NodeConsole"), m_consoleBtn->isChecked());
}


bool SettingsPage::isAutoRun() const
{
  QSettings reg(LS("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
  const QString value = reg.value(kApp, QString()).toString();
  if (value.isEmpty())
    return false;

  return QApplication::applicationFilePath() + LS(" -hide") == QDir::fromNativeSeparators(value);
}


void SettingsPage::retranslateUi()
{
  m_autoRun->setText(tr("Run at Windows start up"));
  m_consoleBtn->setText(tr("Enable remote console (restart required)"));
}
