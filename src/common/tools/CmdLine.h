/* $Id: CmdLine.h 3732 2013-07-07 18:55:44Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#ifndef CMDLINE_H_
#define CMDLINE_H_

#include <QVariant>
#include <QStringList>

#include "schat.h"

class SCHAT_EXPORT CmdLine
{
private:
  CmdLine() {}

public:
  static bool has(const QString &key);
  static bool has(const QStringList &keys);
  static inline QStringList& args()                { return m_args; }
  static inline void init(const QStringList &args) { m_args = args; }
  static QString pidFile(const QString &base);
  static QVariant value(const QString &key);
  static QVariant value(const QStringList &keys);
  static void createPid(const QString &base);
  static void removePid(const QString &base);

private:
  static QStringList m_args;
};

#endif /* CMDLINE_H_ */
