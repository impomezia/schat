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
#include <QLabel>
#include <QVBoxLayout>

#include "AboutPage.h"
#include "Settings.h"
#include "sglobal.h"
#include "version.h"

AboutPage::AboutPage(Settings *settings, QWidget *parent)
  : QWidget(parent)
  , m_settings(settings)
{

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(new QLabel(QString("<h2>%1 %2</h2>").arg(qApp->applicationName(), qApp->applicationVersion()), this));
  layout->addWidget(new QLabel(QString(SCHAT_COPYRIGHT).replace("(C)", "©"), this));
  layout->addStretch();

  retranslateUi();
}


void AboutPage::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void AboutPage::retranslateUi()
{
}
