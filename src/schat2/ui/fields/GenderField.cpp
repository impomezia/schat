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

#include <QComboBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/fields/GenderField.h"

GenderField::GenderField(QWidget *parent)
  : QWidget(parent)
{
  m_channel = ClientChannel(new Channel(ChatClient::id(), ChatClient::channel()->name()));
  m_channel->setSynced(true);
  m_channel->gender() = ChatClient::channel()->gender().raw();

  m_combo = new QComboBox(this);
  m_combo->addItem(tr("Male"));
  m_combo->addItem(tr("Female"));
  m_combo->setMinimumWidth(m_combo->width());

  m_menu = new QMenu(this);

  addColor(tr("Default"));
  addColor(tr("Black"));
  addColor(tr("Gray"));
  addColor(tr("Green"));
  addColor(tr("Red"));
  addColor(tr("White"));
  addColor(tr("Yellow"));

  m_config = new QToolButton(this);
  m_config->setIcon(SCHAT_ICON(Gear));
  m_config->setMenu(m_menu);
  m_config->setPopupMode(QToolButton::InstantPopup);
  m_config->setStyleSheet(LS("QToolButton::menu-indicator {image:none}"));

  m_toolBar = new QToolBar(this);
  m_toolBar->setIconSize(QSize(16, 16));
  m_toolBar->addWidget(m_config);
  m_toolBar->setStyleSheet("QToolBar { margin:0px; border:0px; }" );

  setState();

  QHBoxLayout *mainLay = new QHBoxLayout(this);
  mainLay->addWidget(m_combo);
  mainLay->addWidget(m_toolBar);
  mainLay->addStretch();
  mainLay->setMargin(0);
  mainLay->setSpacing(0);

  connect(m_combo, SIGNAL(currentIndexChanged(int)), SLOT(indexChanged(int)));
  connect(ChatCore::settings(), SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));
}


void GenderField::updateData()
{
  ChatClient::channel()->gender() = m_channel->gender().raw();
  ChatCore::settings()->setValue("Profile/Gender", m_channel->gender().raw());

  if (ChatClient::state() == ChatClient::Online)
    ChatClient::channels()->update();
}


void GenderField::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void GenderField::indexChanged(int index)
{
  if (index == -1)
    return;

  if (index == 0)
    m_channel->gender().set(Gender::Male);
  else
    m_channel->gender().set(Gender::Female);

  setState();
  updateData();
}


void GenderField::setColor()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (!action)
    return;

  m_channel->gender().setColor(action->data().toInt());

  setState();
  updateData();
}


void GenderField::settingsChanged(const QString &key, const QVariant &value)
{
  if (key == "Profile/Gender") {
    m_channel->gender().setRaw(value.toInt());
    setState();
  }
}


void GenderField::addColor(const QString &name)
{
  QAction *action = m_menu->addAction(name, this, SLOT(setColor()));
  action->setCheckable(true);
  action->setData(m_colors.size());
  m_colors.append(action);
}


void GenderField::retranslateUi()
{
  m_combo->setItemText(0, tr("Male"));
  m_combo->setItemText(1, tr("Female"));

  m_colors.at(0)->setText(tr("Default"));
  m_colors.at(1)->setText(tr("Black"));
  m_colors.at(2)->setText(tr("Gray"));
  m_colors.at(3)->setText(tr("Green"));
  m_colors.at(4)->setText(tr("Red"));
  m_colors.at(5)->setText(tr("White"));
  m_colors.at(6)->setText(tr("Yellow"));
}


void GenderField::setIcons()
{
  Gender &gender = m_channel->gender();
  int raw = gender.raw();

  gender.set(Gender::Male);
  m_combo->setItemIcon(0, ChatIcons::icon(m_channel, ChatIcons::NoOptions));

  gender.set(Gender::Female);
  m_combo->setItemIcon(1, ChatIcons::icon(m_channel, ChatIcons::NoOptions));

  gender.setRaw(raw);

  for (int i = 0; i < m_colors.size(); ++i) {
    gender.setColor(i);
    m_colors.at(i)->setIcon(ChatIcons::icon(m_channel, ChatIcons::Statuses));
  }

  gender.setRaw(raw);
}


void GenderField::setState()
{
  setIcons();
  m_config->setEnabled(true);

  const Gender &gender = m_channel->gender();
  if (gender.value() == Gender::Female)
    m_combo->setCurrentIndex(1);
  else if (gender.value() == Gender::Male)
    m_combo->setCurrentIndex(0);
  else {
    m_combo->setCurrentIndex(-1);
    m_config->setEnabled(false);
    return;
  }

  if (gender.color() > Gender::Yellow)
    return;

  for (int i = 0; i < m_colors.size(); ++i)
    m_colors.at(i)->setChecked(false);

  m_colors.at(gender.color())->setChecked(true);
}
