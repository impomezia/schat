/* $Id: benchmarkapp.cpp 2121 2012-01-08 09:37:09Z IMPOMEZIA $
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

#include <QDebug>
#include <QTextCodec>
#include <QTimer>

#include "benchmark.h"
#include "benchmarkapp.h"
#include "version.h"

#if !defined(BENCHMARK_NO_UI)
  #include "benchmarkui.h"
#endif

/*!
 * Конструктор класса BenchmarkApp.
 */
BenchmarkApp::BenchmarkApp(int &argc, char **argv)
#if !defined(BENCHMARK_NO_UI)
  : QApplication(argc, argv),
#else
  : QCoreApplication(argc, argv),
#endif
  m_benchmark(0)
{
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

  setApplicationName("Simple Chat Benchmark");
  setApplicationVersion(SCHAT_VERSION);
  setOrganizationName(SCHAT_ORGANIZATION);
  setOrganizationDomain(SCHAT_DOMAIN);
  addLibraryPath(applicationDirPath() + "/plugins");

  #if !defined(BENCHMARK_NO_UI)
  m_ui = new BenchmarkUi();
  connect(m_ui, SIGNAL(start()), this, SLOT(start()));
  connect(m_ui, SIGNAL(stop()), this, SLOT(stop()));

  m_ui->show();
  #else
  QTimer::singleShot(0, this, SLOT(start()));
  #endif
}


BenchmarkApp::~BenchmarkApp()
{
}


void BenchmarkApp::start()
{
  if (m_benchmark)
    return;

  m_benchmark = new Benchmark(this);
  #if !defined(BENCHMARK_NO_UI)
  connect(m_benchmark, SIGNAL(started(int)), m_ui, SLOT(started(int)));
  connect(m_benchmark, SIGNAL(rejected(int)), m_ui, SLOT(rejected(int)));
  connect(m_benchmark, SIGNAL(accepted(int)), m_ui, SLOT(accepted(int)));
  connect(m_benchmark, SIGNAL(synced(int)), m_ui, SLOT(synced(int)));
  connect(m_benchmark, SIGNAL(disconnected(int)), m_ui, SLOT(disconnected(int)));
  #endif

  m_benchmark->start();
}


void BenchmarkApp::stop()
{
  if (!m_benchmark)
    return;

  m_benchmark->quit();
//  m_benchmark->deleteLater();
//  m_benchmark = 0;
}
