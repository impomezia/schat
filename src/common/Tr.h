/* $Id: Tr.h 2502 2012-04-06 15:37:36Z IMPOMEZIA $
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

#ifndef TR_H_
#define TR_H_

#include <QString>
#include <QList>

#include "schat.h"

class SCHAT_EXPORT Tr
{
public:
  Tr();
  virtual ~Tr();
  inline const QString& prefix() const { return m_prefix; }

  inline static void add(Tr *hook)     { m_hooks.append(hook); }
  inline static void remove(Tr *hook)  { m_hooks.removeAll(hook); }
  static QString value(const QString &key);

protected:
  virtual QString valueImpl(const QString &key) const;

  QString m_prefix;          ///< Префикс переводов.

private:
  static QList<Tr*> m_hooks; ///< Хуки.
};

#endif /* TR_H_ */
