/* $Id: TextField.cpp 2535 2012-04-12 22:51:35Z IMPOMEZIA $
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

#include <QVariant>
#include <QVBoxLayout>

#include "arora/lineedit.h"
#include "ui/profile/TextField.h"
#include "ui/Spinner.h"

TextField::TextField(const QString &field, QWidget *parent)
  : ProfileField(field, parent)
{
  m_edit = new LineEdit(this);
  m_spinner = new Spinner(this);

  m_edit->addWidget(m_spinner, LineEdit::RightSide);
  QVBoxLayout *mainLay = new QVBoxLayout(this);
  mainLay->setMargin(0);
  mainLay->setSpacing(0);
  mainLay->addWidget(m_edit);

  connect(m_edit, SIGNAL(editingFinished()), SLOT(editingFinished()));
}


void TextField::editingFinished()
{
  if (apply(m_edit->text()))
    m_spinner->start();
}


void TextField::setData(const QVariant &value)
{
  m_spinner->stop();
  m_edit->setText(value.toString());
}
