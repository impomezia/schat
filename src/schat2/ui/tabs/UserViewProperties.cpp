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
#include <QCheckBox>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "UserViewProperties.h"

UserViewProperties::UserViewProperties(QWidget *parent)
  : DialogFrame(parent)
  , m_settings(ChatCore::settings())
{
  m_layout->addWidget(line());

  m_offlineBtn = new QCheckBox(this);
  m_offlineBtn->setChecked(m_settings->value(ChatSettings::kUserListOffline).toBool());

  m_keyboardBtn = new QCheckBox(this);
  m_keyboardBtn->setChecked(m_settings->value(ChatSettings::kUserListKeyboard).toBool());

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(m_offlineBtn);
  layout->addWidget(m_keyboardBtn);
  layout->setContentsMargins(0, 10, 0, 0);
  layout->setSpacing(10);

  m_layout->addLayout(layout);

  retranslateUi();

  connect(m_offlineBtn, SIGNAL(clicked()), SLOT(save()));
  connect(m_keyboardBtn, SIGNAL(clicked()), SLOT(save()));
}


void UserViewProperties::retranslateUi()
{
  setTitle(tr("User list properties"));

  m_offlineBtn->setText(tr("Show offline users"));
  m_keyboardBtn->setText(tr("Keyboard navigation"));
}


void UserViewProperties::save()
{
  m_settings->setValue(ChatSettings::kUserListOffline,  m_offlineBtn->isChecked());
  m_settings->setValue(ChatSettings::kUserListKeyboard, m_keyboardBtn->isChecked());
}
