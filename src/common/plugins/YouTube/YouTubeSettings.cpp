/* $Id: YouTubeSettings.cpp 3288 2012-11-28 05:48:24Z IMPOMEZIA $
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

#include <QEvent>
#include <QCheckBox>
#include <QVBoxLayout>

#include "ChatCore.h"
#include "YouTubeSettings.h"
#include "ChatSettings.h"
#include "sglobal.h"

YouTubeSettings::YouTubeSettings(QWidget *parent)
  : QWidget(parent)
{
  m_embed = new QCheckBox(this);
  m_embed->setChecked(ChatCore::settings()->value(LS("YouTube/EmbedVideo")).toBool());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_embed);
  layout->setMargin(0);

  retranslateUi();

  connect(m_embed, SIGNAL(clicked(bool)), SLOT(embed(bool)));
}


void YouTubeSettings::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void YouTubeSettings::embed(bool checked)
{
  ChatCore::settings()->setValue(LS("YouTube/EmbedVideo"), checked);
}


void YouTubeSettings::retranslateUi()
{
  m_embed->setText(tr("Embed video"));
}
