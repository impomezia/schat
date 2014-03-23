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

#ifndef HASHRUNNABLE_H_
#define HASHRUNNABLE_H_

#include <QObject>
#include <QRunnable>

#include "schat.h"

class HashRunnable : public QObject, public QRunnable
{
  Q_OBJECT

public:
  HashRunnable(const QString &fileName);
  void run() Q_DECL_OVERRIDE;

signals:
  void finished(const QByteArray &hash);

private:
  const QString m_fileName;
};

#endif // HASHRUNNABLE_H_
