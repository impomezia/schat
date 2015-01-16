/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QToolButton>

#include "alerts/AlertsWidget.h"
#include "hooks/SettingsTabImpl.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/fields/GenderField.h"
#include "ui/fields/LanguageField.h"
#include "ui/fields/NickEdit.h"
#include "ui/network/NetworkWidget.h"
#include "ui/profile/ProfileLayout.h"

ProfilePage::ProfilePage(QWidget *parent)
  : SettingsPage(SCHAT_ICON(Profile), LS("profile"), parent)
  , m_autoRunBtn(0)
{
  m_profileLabel = new QLabel(this);
  m_nickLabel = new QLabel(this);
  m_nickEdit = new NickEdit(this);

  m_genderLabel = new QLabel(this);
  m_genderField = new GenderField(this);

  m_layout = new ProfileLayout(this);

  QGridLayout *profileLay = new QGridLayout();
  profileLay->addWidget(m_nickLabel, 0, 0);
  profileLay->addWidget(m_nickEdit, 0, 1, 1, 2);
  profileLay->addWidget(m_genderLabel, 1, 0);
  profileLay->addWidget(m_genderField, 1, 1);
  profileLay->addWidget(m_layout->button(), 1, 2);
  profileLay->setContentsMargins(10, 0, 3, 0);

  m_mainLayout = new QVBoxLayout();
  m_mainLayout->addWidget(m_profileLabel);
  m_mainLayout->addLayout(profileLay);
  m_mainLayout->addWidget(m_layout);

  setupLayout();

# ifdef Q_OS_WIN
  m_autoRunBtn = new QCheckBox(this);
  m_autoRunBtn->setChecked(isAutoRun());
  static_cast<QVBoxLayout*>(layout())->addWidget(m_autoRunBtn);

  connect(m_autoRunBtn, SIGNAL(clicked(bool)), SLOT(onAutoRunClicked(bool)));
# endif

  retranslateUi();
}


void ProfilePage::onAutoRunClicked(bool checked)
{
  QSettings reg(LS("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
  if (checked) {
    QStringList args = QApplication::arguments();
    args.takeFirst();

    reg.setValue(LS("Simple Chat 2"), QDir::toNativeSeparators(QApplication::applicationFilePath()) + LS(" -hide ") + args.join(LC(' ')));
  }
  else
    reg.remove(LS("Simple Chat 2"));

  reg.remove(QApplication::applicationName());
}


bool ProfilePage::isAutoRun() const
{
  QSettings reg(LS("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
  const QString value = reg.value(LS("Simple Chat 2"), QString()).toString();
  if (value.isEmpty())
    return false;

  QStringList args = QApplication::arguments();
  args.takeFirst();

  return QApplication::applicationFilePath() + LS(" -hide ") + args.join(LC(' ')) == QDir::fromNativeSeparators(value);
}


void ProfilePage::retranslateUi()
{
  m_name = tr("Profile");

  m_profileLabel->setText(LS("<b>") + m_name + LS("</b>"));
  m_nickLabel->setText(tr("Nick:"));
  m_genderLabel->setText(tr("Gender:"));

  m_layout->retranslateUi();

  if (m_autoRunBtn)
    m_autoRunBtn->setText(tr("Run at Windows start up"));
}


NetworkPage::NetworkPage(QWidget *parent)
  : SettingsPage(SCHAT_ICON(Globe), LS("network"), parent)
{
  m_networkLabel = new QLabel(this);
  m_network = new NetworkWidget(this);

  QVBoxLayout *networkLay = new QVBoxLayout();
  networkLay->addWidget(m_network);
  networkLay->setContentsMargins(10, 0, 3, 0);

  m_mainLayout = new QVBoxLayout();
  m_mainLayout->addWidget(m_networkLabel);
  m_mainLayout->addLayout(networkLay);

  setupLayout();
  retranslateUi();
}


void NetworkPage::retranslateUi()
{
  m_name = tr("Network");
  m_networkLabel->setText(LS("<b>") + m_name + LS("</b>"));
}


AlertsPage::AlertsPage(QWidget *parent)
  : SettingsPage(SCHAT_ICON(Bell), LS("alerts"), parent)
{
  m_label = new QLabel(this);
  m_alertsWidget = new AlertsWidget(this);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(m_alertsWidget);
  layout->setContentsMargins(10, 0, 3, 0);

  m_mainLayout = new QVBoxLayout();
  m_mainLayout->addWidget(m_label);
  m_mainLayout->addLayout(layout);

  setupLayout();
  retranslateUi();
}


void AlertsPage::retranslateUi()
{
  m_name = tr("Notifications");
  m_label->setText(LS("<b>") + m_name + LS("</b>"));
}


LocalePage::LocalePage(QWidget *parent)
  : SettingsPage(SCHAT_ICON(Locale), LS("locale"), parent)
{
  m_localeLabel = new QLabel(this);

  m_language = new LanguageField(this);
  m_language->setMinimumWidth(m_language->width() + 20);

  QHBoxLayout *languageLay = new QHBoxLayout();
  languageLay->addWidget(m_language);
  languageLay->addStretch();
  languageLay->setContentsMargins(10, 0, 3, 0);

  m_mainLayout = new QVBoxLayout();
  m_mainLayout->addWidget(m_localeLabel);
  m_mainLayout->addLayout(languageLay);

  setupLayout();
  retranslateUi();
}


void LocalePage::retranslateUi()
{
  m_name = tr("Language");
  m_localeLabel->setText(LS("<b>") + m_name + LS("</b>"));
}
