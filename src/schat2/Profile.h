/* $Id: Profile.h 2504 2012-04-06 18:33:11Z IMPOMEZIA $
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

#ifndef PROFILE_H_
#define PROFILE_H_

#include <QMap>
#include <QObject>

#include "schat.h"

class ProfileTr;

class SCHAT_CORE_EXPORT Profile : public QObject
{
  Q_OBJECT

public:
  Profile(QObject *parent = 0);
  ~Profile();
  inline static Profile *i()               { return m_self; }
  inline static void add(Profile *hook)    { if (!m_self->m_hooks.contains(hook)) m_self->m_hooks.append(hook); }
  inline static void remove(Profile *hook) { m_self->m_hooks.removeAll(hook); }

  static QString translate(const QString &field);
  static QStringList available();
  static QStringList fields();
  static QStringList filled();

  static void addField(const QString &name, int weight);

protected:
  virtual QString translateImpl(const QString &field) const;

private slots:
  void ready();

private:
  ProfileTr *m_tr;                    ///< Класс перевода строк.
  QList<Profile*> m_hooks;            ///< Хуки.
  static Profile *m_self;             ///< Указатель на себя.
  static QMap<int, QString> m_fields; ///< Поддерживаемые поля.
};

#endif /* PROFILE_H_ */
