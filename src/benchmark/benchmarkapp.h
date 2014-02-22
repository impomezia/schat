/* $Id: benchmarkapp.h 1356 2010-12-11 17:23:28Z IMPOMEZIA $
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

#ifndef BENCHMARKAPP_H_
#define BENCHMARKAPP_H_

class Benchmark;
class BenchmarkUi;

/*!
 * \brief Экземпляр приложения.
 */
#if !defined(BENCHMARK_NO_UI)
#include <QApplication>
class BenchmarkApp : public QApplication
#else
#include <QCoreApplication>
class BenchmarkApp : public QCoreApplication
#endif
{
  Q_OBJECT

public:
  BenchmarkApp(int &argc, char **argv);
  ~BenchmarkApp();

private slots:
  void start();
  void stop();

private:
  Benchmark *m_benchmark;
  BenchmarkUi *m_ui;
};

#endif /* BENCHMARKAPP_H_ */
