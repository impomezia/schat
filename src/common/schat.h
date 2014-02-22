/* $Id: schat.h 2991 2012-08-10 10:41:40Z IMPOMEZIA $
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

#ifndef SCHAT_H_
#define SCHAT_H_

#if defined(SCHAT_NO_DLL)
#  define SCHAT_EXPORT
#  define SCHAT_CORE_EXPORT
#else
#  if defined(SCHAT_LIBRARY)
#    define SCHAT_EXPORT Q_DECL_EXPORT
#  else
#    define SCHAT_EXPORT Q_DECL_IMPORT
#  endif
#  if defined(SCHAT_CORE_LIBRARY)
#    define SCHAT_CORE_EXPORT Q_DECL_EXPORT
#  else
#    define SCHAT_CORE_EXPORT Q_DECL_IMPORT
#  endif
#  if defined(SCHAT_REST_LIBRARY)
#    define SCHAT_REST_EXPORT Q_DECL_EXPORT
#  else
#    define SCHAT_REST_EXPORT Q_DECL_IMPORT
#  endif
#endif

#endif /* SCHAT_H_ */
