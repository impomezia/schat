/* $Id: ProfileLayout.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QTimer>
#include <QToolButton>

#include "ChatNotify.h"
#include "Profile.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/profile/ProfileField.h"
#include "ui/profile/ProfileFieldFactory.h"
#include "ui/profile/ProfileLayout.h"

ProfileLayout::ProfileLayout(QWidget *parent)
  : QWidget(parent)
{
  m_menu = new QMenu(this);
  m_button = new QToolButton(this);
  m_button->setMenu(m_menu);
  m_button->setText(tr("Add") + LS(" "));
  m_button->setIcon(SCHAT_ICON(Add));
  m_button->setPopupMode(QToolButton::InstantPopup);
  m_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  m_layout = new QGridLayout(this);
  m_layout->setContentsMargins(10, 10, 3, 0);
  m_layout->setColumnStretch(1, 1);

  connect(m_menu, SIGNAL(aboutToShow()), SLOT(menuAboutToShow()));
  connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(menuTriggered(QAction*)));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));

  QTimer::singleShot(0, this, SLOT(reload()));
}


void ProfileLayout::retranslateUi()
{
  foreach (ProfileField *field, m_fields) {
    field->retranslateUi();
  }
}


/*!
 * Добавления поля редактирования профиля.
 */
void ProfileLayout::add(const QString &field)
{
  if (m_fields.contains(field))
    return;

  int index = Profile::fields().indexOf(field);
  if (index == -1)
    return;

  ProfileField *widget = ProfileFieldFactory::create(field, this);
  if (!widget)
    return;

  m_fields[field] = widget;

  m_layout->addWidget(widget->label(), index, 0);
  m_layout->addWidget(widget, index, 1);

  m_button->setVisible(!available().isEmpty());
}


void ProfileLayout::menuAboutToShow()
{
  m_menu->clear();
  QStringList fields = available();
  if (fields.isEmpty())
    return;

  foreach (QString field, fields) {
    QAction *action = m_menu->addAction(Profile::translate(field));
    action->setData(field);
  }
}


void ProfileLayout::menuTriggered(QAction *action)
{
  if (action)
    add(action->data().toString());
}


void ProfileLayout::notify(const Notify &notify)
{
  if (ProfileField::isMatch(notify))
    reload();
}


void ProfileLayout::reload()
{
  QStringList filled = Profile::filled();
  foreach (QString field, filled) {
    add(field);
  }

  if (m_fields.isEmpty())
    return;

  QStringList keys = m_fields.keys();
  for (int i = 0; i < keys.size(); ++i) {
    if (filled.contains(keys.at(i)))
      continue;

    ProfileField *widget = m_fields.value(keys.at(i));
    m_layout->removeWidget(widget->label());
    m_layout->removeWidget(widget);
    widget->label()->deleteLater();
    widget->deleteLater();
    m_fields.remove(keys.at(i));
  }

  m_button->setVisible(!available().isEmpty());
}


QStringList ProfileLayout::available() const
{
  QStringList out = Profile::available();
  foreach (QString field, m_fields.keys()) {
    out.removeAll(field);
  }

  return out;
}
