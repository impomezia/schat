/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#include <qmath.h>

#include "crc8.h"
#include "ShortID.h"

static const char alphabet[] = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";
static const int base        = sizeof(alphabet) - 1;


bool ShortID::isValid(const QString &id)
{
  const int size = id.size();
  if (size < 3)
    return false;

  const QByteArray data = id.toLatin1();

  Q_ASSERT(data.size() == size);

  if (data.size() != size)
    return false;

  const QByteArray ab = QByteArray::fromRawData(alphabet, base);
  int index;
  int num = 0;

  for (int i = size; i-- > size - 2;) {
    if ((index = ab.indexOf(data.at(i))) == -1)
      return false;

    num += index * qPow(base, size - i - 1);
  }

  return crc8(data.constData(), size - 2) == (num - base);
}


QString ShortID::generate(int size)
{
  Q_ASSERT(size >= 3);

  if (size < 3)
    size = 3;

  QByteArray data;
  data.reserve(size);

  for (int i = 0; i < size - 2; i++) {
    data[i] = alphabet[(qrand() % base)];
  }

  const int num = crc8(data.constData(), size - 2) + base;
  const int mod = num % base;
  data[size - 1] = alphabet[mod];
  data[size - 2] = alphabet[((num - mod) / base)];

  return QString::fromLatin1(data.constData(), size);
}
