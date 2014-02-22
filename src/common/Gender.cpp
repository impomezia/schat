/* $Id: Gender.cpp 2950 2012-07-31 06:06:11Z IMPOMEZIA $
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

#include "Gender.h"
#include "sglobal.h"

quint8 Gender::color() const
{
  if (m_gender >= Unknown)
    return Default;

  quint8 out = m_gender;
  if (value() == Female)
    out -= Female;

  if (out > Thief)
    return Default;

  return out;
}


quint8 Gender::value() const
{
  if (m_gender >= Unknown && m_gender <= Bot)
    return m_gender;

  if (m_gender > Bot)
    return Unknown;

  if (m_gender < Female)
    return Male;

  return Female;
}


/*!
 * Установка пола.
 */
void Gender::set(Genders gender)
{
  if (gender >= Unknown)
    m_gender = gender;
  else
    m_gender = gender + color();
}


void Gender::setColor(Colors color)
{
  if (m_gender >= Unknown)
    return;

  m_gender = value() + color;
}


void Gender::setColor(quint8 color)
{
  if (color > Thief)
    color = Default;

  setColor(static_cast<Colors>(color));
}


int Gender::stringToColor(const QString &color)
{
  QString c = color.toLower();
  if (c == LS("black"))
    return Black;

  if (c == LS("gray"))
    return Gray;

  if (c == LS("green"))
    return Green;

  if (c == LS("red"))
    return Red;

  if (c == LS("white"))
    return White;

  if (c == LS("yellow"))
    return Yellow;

  if (c == LS("medical"))
    return Medical;

  if (c == LS("nude"))
    return Nude;

  if (c == LS("thief"))
    return Thief;

  return Default;
}


QString Gender::colorToString(quint8 color)
{
  switch (color) {
    case Black:
      return LS("black");

    case Gray:
      return LS("gray");

    case Green:
      return LS("green");

    case Red:
      return LS("red");

    case White:
      return LS("white");

    case Yellow:
      return LS("yellow");

    case Medical:
      return LS("medical");

    case Nude:
      return LS("nude");

    case Thief:
      return LS("thief");

    default:
      return LS("default");
      break;
  }
}


Gender& Gender::operator=(int other)
{
  m_gender = other;
  return *this;
}
