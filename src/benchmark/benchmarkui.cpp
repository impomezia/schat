/* $Id: benchmarkui.cpp 1358 2010-12-12 16:28:41Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2010 IMPOMEZIA <schat@impomezia.com>
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
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

#include "benchmarkui.h"

/*!
 * \brief Конструктор класса AboutDialog.
 */
BenchmarkUi::BenchmarkUi(QWidget *parent)
  : QDialog(parent)
{
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);

  setWindowTitle(QApplication::applicationName() + " " + QApplication::applicationVersion());

  m_start = new QPushButton(tr("&Start"), this);
  m_start->setDefault(true);
  connect(m_start, SIGNAL(clicked(bool)), this, SLOT(start(bool)));

  m_stop = new QPushButton(tr("S&top"), this);
  m_stop->setEnabled(false);
  connect(m_stop, SIGNAL(clicked(bool)), this, SLOT(stop(bool)));

  m_statistics = new QGroupBox(tr("Statistics"), this);
  m_started = new QLabel("0", this);
  m_rejected = new QLabel("0", this);
  m_accepted = new QLabel("0", this);
  m_synced = new QLabel("0", this);
  m_disconnected = new QLabel("0", this);

  QHBoxLayout *controlLay = new QHBoxLayout();
  controlLay->addWidget(m_start);
  controlLay->addWidget(m_stop);

  QFormLayout *statisticsLay = new QFormLayout(m_statistics);
  statisticsLay->addRow("Started:", m_started);
  statisticsLay->addRow("Rejected:", m_rejected);
  statisticsLay->addRow("Accepted:", m_accepted);
  statisticsLay->addRow("Synced:", m_synced);
  statisticsLay->addRow("Disconnected:", m_disconnected);

  QVBoxLayout *mainLay = new QVBoxLayout(this);
  mainLay->addLayout(controlLay);
  mainLay->addWidget(m_statistics);
}


void BenchmarkUi::start(bool checked)
{
  Q_UNUSED(checked);
  m_start->setEnabled(false);
  m_stop->setEnabled(true);
  m_started->setText("0");
  m_rejected->setText("0");
  m_accepted->setText("0");
  m_synced->setText("0");
  m_disconnected->setText("0");
  emit start();
}


void BenchmarkUi::stop(bool checked)
{
  Q_UNUSED(checked);
  m_start->setEnabled(true);
  m_stop->setEnabled(false);
  emit stop();
}
