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

#ifndef SENDFILEDB_H_
#define SENDFILEDB_H_

#include <QObject>

#include "SendFileTransaction.h"

class SendFileDB : public QObject
{
  Q_OBJECT

public:
  SendFileDB(QObject *parent = 0);
  ~SendFileDB();
  bool open(const QByteArray &id, const QString &dir);
  inline const QString& id() const { return m_id; }
  void close();
  void restore(const QByteArray &id, SendFileTransaction transaction);
  void save(const SendFileTransaction &transaction);

private:
  void create();
  void version();

  QString m_id; ///< Идентификатор соединения с базой.
};

#endif /* SENDFILEDB_H_ */
