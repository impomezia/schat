/* $Id: NetworkButton.cpp 2250 2012-02-03 06:16:09Z IMPOMEZIA $
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

#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

#include "QProgressIndicator/QProgressIndicator.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/network/NetworkButton.h"

NetworkButton::NetworkButton(const QString &text, QWidget *parent)
  : QWidget(parent)
{
  m_button = new QToolButton(this);
  m_button->setIcon(SCHAT_ICON(OK));
  m_button->setText(text);
  m_button->setToolTip(text);
  m_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  m_progress = new QProgressIndicator(this);
  m_progress->setAnimationDelay(100);
  m_progress->setMaximumSize(16, 16);
  m_progress->setVisible(false);

  m_error = new QLabel(this);
  m_error->setPixmap(QPixmap(LS(":/images/exclamation-red.png")));
  m_error->setVisible(false);

  QHBoxLayout *mainLay = new QHBoxLayout(this);
  mainLay->addWidget(m_progress);
  mainLay->addWidget(m_error);
  mainLay->addWidget(m_button);
  mainLay->setMargin(0);
  mainLay->setSpacing(0);
}


void NetworkButton::setError(const QString &error)
{
  m_progress->stopAnimation();
  m_progress->setVisible(false);
  m_error->setVisible(false);
  m_error->setToolTip(error);
  m_error->setVisible(true);
  setEnabled(true);
}


void NetworkButton::setProgress()
{
  m_button->setVisible(false);
  m_error->setVisible(false);
  m_progress->startAnimation();
  m_progress->setVisible(true);
  setEnabled(true);
}


void NetworkButton::setReady(bool ready)
{
  m_progress->stopAnimation();
  m_progress->setVisible(false);
  m_error->setVisible(false);
  m_button->setVisible(true);
  setEnabled(ready);
}


void NetworkButton::setText(const QString &text)
{
  m_button->setText(text);
  m_button->setToolTip(text);
}
