/* $Id: DateTime.cpp 2994 2012-08-11 19:46:55Z IMPOMEZIA $
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

#include "DateTime.h"

/*!
 * Возвращает смещение в секундах относительно UTC.
 */
int DateTime::tz()
{
  QDateTime local = QDateTime::currentDateTime();
  QDateTime utc(local);
  utc.setTimeSpec(Qt::UTC);
  return local.secsTo(utc);
}


QDateTime DateTime::toDateTime(qint64 date)
{
# if QT_VERSION >= 0x040700
  return QDateTime::fromMSecsSinceEpoch(date);
# else
  qint64 t = date / 1000;
  return QDateTime::fromTime_t(t).addMSecs(date - t * 1000);
# endif
}


/*!
 * Возвращает число миллисекунд с 1970-01-01T00:00:00 UTC.
 */
qint64 DateTime::utc()
{
# if QT_VERSION >= 0x040700
  return QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
# else
  QDateTime dt = QDateTime::currentDateTime().toUTC();
  qint64 t = dt.toTime_t();
  t *= 1000;
  t += dt.time().msec();
  return t;
# endif
}
