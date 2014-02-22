/* $Id: BgOperationWidget.cpp 3377 2013-01-06 14:38:05Z IMPOMEZIA $
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

#include <QProgressBar>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

#include "ui/BgOperationWidget.h"

BgOperationWidget *BgOperationWidget::m_self = 0;

BgOperationWidget::BgOperationWidget(QWidget *parent)
  : QWidget(parent)
{
  m_self = this;

  m_label = new QLabel(this);
  m_label->setVisible(false);
  m_label->setAttribute(Qt::WA_TransparentForMouseEvents);

  m_progress = new QProgressBar(this);
  m_progress->setMaximumHeight(16);
  m_progress->setTextVisible(false);
  m_progress->setVisible(false);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(m_label);
  layout->addWidget(m_progress);
  layout->setMargin(0);
}


bool BgOperationWidget::lock(const QString &key)
{
  if (!m_self)
    return false;

  if (m_self->m_lockKey.isEmpty()) {
    m_self->m_lockKey = key;
    return true;
  }

  if (m_self->m_lockKey == key)
    return true;

  return false;
}


bool BgOperationWidget::lock(const QString &key, const QString &text)
{
  if (!lock(key))
    return false;

  m_self->m_label->setVisible(true);
  m_self->m_label->setText(text);
  return true;
}


bool BgOperationWidget::unlock(const QString &key, bool reset)
{
  if (!m_self || m_self->m_lockKey != key)
    return false;

  m_self->m_lockKey.clear();

  if (reset) {
    m_self->m_label->setVisible(false);
    m_self->m_label->setText(QString());
    m_self->m_progress->setVisible(false);
  }

  return true;
}


void BgOperationWidget::setText(const QString &text)
{
  if (!m_self)
    return;

  m_self->m_label->setVisible(!text.isEmpty());
  m_self->m_label->setText(text);
}


void BgOperationWidget::mouseReleaseEvent(QMouseEvent *event)
{
  emit clicked(m_lockKey, event);

  QWidget::mouseReleaseEvent(event);
}
