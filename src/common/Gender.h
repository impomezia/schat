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

#ifndef GENDER_H_
#define GENDER_H_

#include <QString>

#include "schat.h"

/*!
 * Пол и цвет пользователя.
 */
class SCHAT_EXPORT Gender
{
public:
  /// Пол пользователя.
  enum Genders {
    Male    = 0,   ///< Мужской пол.
    Female  = 100, ///< Женский пол.
    Unknown = 150, ///< Неизвестный пол.
    Ghost   = 151, ///< Привидение.
    Bot     = 152  ///< Бот.
  };

  /// Цветовой вариант иконки пола.
  enum Colors {
    Default,
    Black,
    Gray,
    Green,
    Red,
    White,
    Yellow,
    Medical,
    Nude,
    Thief
  };

  Gender(quint8 gender = Male)
  : m_gender(gender)
  {}

  inline int raw() const          { return m_gender; }
  inline void setRaw(int gender)  { m_gender = gender; }
  quint8 color() const;
  quint8 value() const;
  void set(const QVariantMap &data);
  void setColor(Colors color);

private:
  void set(const QString &gender, const QString &color);

  quint8 m_gender;
};

#endif /* GENDER_H_ */
