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

#include <QFrame>
#include <QHBoxLayout>

#include "ChatCore.h"
#include "QProgressIndicator/QProgressIndicator.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/ProgressTab.h"

ProgressTab::ProgressTab(TabWidget *parent)
  : AbstractTab(PROGRESS_TAB, LS(PROGRESS_TAB), parent)
{
  QFrame *frame = new QFrame(this);
# if defined(Q_OS_WIN32)
  frame->setStyleSheet(LS("QFrame{background-color:#fff;}"));
# endif

  m_progress = new QProgressIndicator(frame);
  m_progress->setAnimationDelay(100);
  m_progress->setMinimumSize(100, 100);
  m_progress->startAnimation();

  QHBoxLayout *mainLay = new QHBoxLayout(this);
  mainLay->addWidget(frame);
  mainLay->setMargin(0);

  QHBoxLayout *frameLay = new QHBoxLayout(frame);
  frameLay->addWidget(m_progress, Qt::AlignCenter);

  setIcon(SCHAT_ICON(SmallLogo));
  retranslateUi();
}


void ProgressTab::retranslateUi()
{
  setText(tr("Connecting"));
}


AbstractTab *ProgressTabCreator::create(const QVariant &data, TabWidget *parent) const
{
  Q_UNUSED(data)
  return new ProgressTab(parent);
}


void ProgressTabCreator::reload(AbstractTab *tab, const QVariant &data) const
{
  Q_UNUSED(tab)
  Q_UNUSED(data);
}
