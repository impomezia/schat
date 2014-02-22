/* $Id: UpdateSettings.cpp 3763 2013-08-09 15:05:39Z IMPOMEZIA $
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

#include <QCheckBox>
#include <QVBoxLayout>
#include <QEvent>

#include "ChatCore.h"
#include "UpdateSettings.h"
#include "ChatSettings.h"
#include "sglobal.h"
#include "Path.h"

UpdateSettings::UpdateSettings(QWidget *parent)
  : QWidget(parent)
{
  m_autoDownload = new QCheckBox(this);
  m_autoDownload->setChecked(ChatCore::settings()->value(LS("Update/AutoDownload")).toBool());

  m_beta = new QCheckBox(this);
  m_beta->setChecked(ChatCore::settings()->value(LS("Update/Channel")) == LS("beta"));

  if (!Path::isPortable()) {
    m_autoDownload->setChecked(false);
    m_autoDownload->setEnabled(false);
  }

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_autoDownload);
# if defined(Q_OS_WIN)
  layout->addWidget(m_beta);
# else
  m_beta->hide();
# endif
  layout->setMargin(0);

  retranslateUi();

  connect(m_autoDownload, SIGNAL(clicked(bool)), SLOT(save()));
  connect(m_beta, SIGNAL(clicked(bool)), SLOT(save()));
}


void UpdateSettings::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void UpdateSettings::save()
{
  ChatCore::settings()->setValue(LS("Update/AutoDownload"), m_autoDownload->isChecked());
  ChatCore::settings()->setValue(LS("Update/Channel"), m_beta->isChecked() ? LS("beta") : LS("stable"));
}


void UpdateSettings::retranslateUi()
{
  m_autoDownload->setText(tr("Automatically download and install updates"));
  m_beta->setText(tr("Update to beta versions"));
}
