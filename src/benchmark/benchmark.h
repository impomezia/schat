/* $Id: benchmark.h 2130 2012-01-11 04:09:27Z IMPOMEZIA $
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

#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <QObject>
#include <QVariant>

/*!
 * \brief Класс для стресс-тестирования.
 */
class Benchmark : public QObject
{
  Q_OBJECT

public:
  Benchmark(QObject *parent = 0);
  ~Benchmark();
  void quit();
  void start();

signals:
  void accepted(int count);
  void disconnected(int count);
  void rejected(int count);
  void started(int count);
  void synced(int count);

private slots:
  inline void accessDenied(quint16) { emit rejected(++m_rejected); }
  inline void accessGranted(const QString &, const QString &, quint16) { emit accepted(++m_accepted); }
  inline void syncUsersEnd() { emit synced(++m_synced); }
  inline void unconnected() { emit disconnected(++m_disconnected); }
  void clientStateChanged(int state, int previousState);
  void connectToHost();

private:
  int m_accepted;               ///< Счётчик принятых соединений.
  int m_connectInterval;        ///< Интервал подключения (ConnectInterval), по умолчанию 200 мс.
  int m_count;                  ///< Счётчик созданных подключений.
  int m_disconnected;           ///< Счётчик отключений.
  int m_rejected;               ///< Счётчик клиентов которым отказано в подключении.
  int m_synced;                 ///< Счётчик синхронизированных соединений.
  int m_usersCount;             ///< Число подключений (UsersCount), по умолчанию 10.
  QString m_nickPrefix;         ///< Префикс имён пользователей (NickPrefix), по умолчанию "test_".
  QString m_serverAddr;         ///< Адрес сервера для тестирования (Url)".
  QVariantList m_authIn;        ///< Авторизационные данные.
  QVariantList m_authOut;       ///< Авторизационные данные.
};

#endif /* BENCHMARK_H_ */
