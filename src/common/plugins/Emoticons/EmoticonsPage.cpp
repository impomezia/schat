/* $Id: EmoticonsPage.cpp 2822 2012-07-03 03:15:13Z IMPOMEZIA $
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

#include <QLabel>
#include <QVBoxLayout>

#include "EmoticonsPage.h"
#include "sglobal.h"

EmoticonsPage::EmoticonsPage(Emoticons *emoticons, QWidget *parent)
  : SettingsPage(QIcon(LS(":/images/Emoticons/icon-small.png")), LS("emoticons"), parent)
  , m_emoticons(emoticons)
{
  m_label = new QLabel(this);

  QVBoxLayout *mainLay = new QVBoxLayout(this);
  mainLay->addWidget(m_label);
  mainLay->addStretch();

  retranslateUi();
}


void EmoticonsPage::retranslateUi()
{
  m_name = tr("Emoticons");

  m_label->setText(LS("<b>") + m_name + LS("</b>"));
}


SettingsPage* EmoticonsPageCreator::page(QWidget *parent)
{
  return new EmoticonsPage(m_emoticons, parent);
}
