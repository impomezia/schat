/* $Id: debugstream.h 2622 2012-05-08 00:47:30Z IMPOMEZIA $
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

#if defined(SCHAT_DEBUG) && !defined(SCHAT_DEBUG_STREAM)
  #define SCHAT_DEBUG_STREAM(x) qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz").toLatin1().constData() << x;
  #include <QDebug>
  #include <QTime>
#else
  #define SCHAT_DEBUG_STREAM(x)
#endif

#if defined(SCHAT_DISABLE_DEBUG) && defined(SCHAT_DEBUG_STREAM)
  #undef SCHAT_DEBUG_STREAM
  #define SCHAT_DEBUG_STREAM(x)
#endif
