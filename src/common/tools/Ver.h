/* $Id: Ver.h 2198 2012-01-22 23:35:46Z IMPOMEZIA $
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

#ifndef VER_H_
#define VER_H_

#include <QString>

#include "schat.h"

/*!
 * Реализует основные операции с версиями в формате "major.minor.patch".
 */
class SCHAT_EXPORT Ver
{
public:
  Ver();
  Ver(const QString &version);
  Ver(quint32 ver);
  inline int major() const { return m_major; }
  inline int minor() const { return m_minor; }
  inline int patch() const { return m_patch; }

  inline bool operator!=(const QString &other) const { return !operator==(other); }
  inline bool operator!=(const Ver &other) const     { return !operator==(other); }
  inline bool operator<(const QString &other) const  { return operator<(Ver(other)); }
  inline bool operator<(const Ver &other) const      { return toUInt() < other.toUInt(); }
  inline bool operator<=(const QString &other) const { return !operator>(other); }
  inline bool operator<=(const Ver &other) const     { return !operator>(other); }
  inline bool operator==(const QString &other) const { return operator==(Ver(other)); };
  inline bool operator==(const Ver &other) const     { return toUInt() == other.toUInt(); }
  inline bool operator>(const QString &other) const  { return Ver(other) < *this; }
  inline bool operator>(const Ver &other) const      { return other < *this; }
  inline bool operator>=(const QString &other) const { return !operator<(other); }
  inline bool operator>=(const Ver &other) const     { return !operator<(other); }

  inline quint32 toUInt() const { return toUInt(*this); }
  QString toString() const;
  void setVersion(const QString &version);

  static quint32 toUInt(const Ver &ver);
  static Ver current();

private:
  int m_major;
  int m_minor;
  int m_patch;
};

#endif /* VER_H_ */
