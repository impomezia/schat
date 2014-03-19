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

#include <QCryptographicHash>
#include <QFile>

#include "HashRunnable.h"

HashRunnable::HashRunnable(const QString &fileName)
  : QObject()
  , m_fileName(fileName)
{
}


void HashRunnable::run()
{
  QFile file(m_fileName);
  if (!file.open(QFile::ReadOnly)) {
    emit finished(QByteArray());
    return;
  }

  QCryptographicHash hash(QCryptographicHash::Sha1);

  QScopedArrayPointer<char> buffer(new char[4096]);
  int length;

  while ((length = file.read(buffer.data(), 4096)) > 0)
    hash.addData(buffer.data(), length);

  emit finished(hash.result());
}
