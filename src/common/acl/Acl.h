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

#ifndef ACL_H_
#define ACL_H_

#include <QMap>
#include <QVariant>

#include "acl/Groups.h"
#include "schat.h"

class Channel;
class Feed;

#define ACL_MASK   QLatin1String("mask")
#define ACL_OWNERS QLatin1String("owners")
#define ACL_OTHERS QLatin1String("others")

class SCHAT_EXPORT Acl
{
public:
  enum Mask {
    Read         = 04,
    Write        = 02,
    Edit         = 01,
    SpecialRead  = 040,
    SpecialWrite = 020,
    SpecialEdit  = 010
  };

  Acl(int acl = 0766);

  bool add(const QByteArray &other, int acl);
  bool get(QVariantMap &data, Channel *channel) const;
  inline const QList<QByteArray> &owners() const     { return m_owners; }
  inline const QMap<QByteArray, int> &others() const { return m_others; }
  inline int mask() const                            { return m_mask; }
  inline void remove(const QByteArray &owner)        { m_owners.removeAll(owner); }
  inline void removeOther(const QByteArray &other)   { m_others.remove(other); }
  inline void setMask(int mask)                      { m_mask = mask; }
  int match(Channel *channel) const;
  void add(const QByteArray &owner);
  void load(const QVariantMap &json);
  void save(QVariantMap &data) const;

private:
  int m_mask;                      ///< Общая маска прав доступа Acl:CommonAcl.
  QMap<QByteArray, int> m_others;  ///< Специальные права доступа для выбранных пользователей.
  QList<QByteArray> m_owners;      ///< Идентификаторы владельцев.
};

#endif /* ACL_H_ */
