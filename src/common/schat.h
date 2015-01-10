/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#ifndef SCHAT_H_
#define SCHAT_H_

#define SCHAT_EXPORT
#define SCHAT_CORE_EXPORT

#ifndef Q_DECL_OVERRIDE
#  define Q_DECL_OVERRIDE
#endif

#define SCHAT_DEBUG_LOG

#ifdef SCHAT_DEBUG_LOG
#  include <QDebug>
#  define SLOG_DEBUG(...)    qDebug()    << "[schat]" << Q_FUNC_INFO << __VA_ARGS__
#  define SLOG_CRITICAL(...) qCritical() << "[schat]" << Q_FUNC_INFO << __VA_ARGS__
#  define SLOG_WARNING(...)  qWarning()  << "[schat]" << Q_FUNC_INFO << __VA_ARGS__
#  define SCHAT_DEBUG_CODE(...) __VA_ARGS__
#else
#  define SLOG_DEBUG(...)
#  define SLOG_CRITICAL(...)
#  define SLOG_WARNING(...)
#  define SCHAT_DEBUG_CODE(...)
#endif

#endif /* SCHAT_H_ */
