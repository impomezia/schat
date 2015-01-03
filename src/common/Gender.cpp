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


void Gender::setColor(Colors color)
{
  if (m_gender >= Unknown)
    return;

  m_gender = value() + color;
}


void Gender::set(const QString &gender, const QString &color)
{
  if (gender == LS("male"))
    m_gender = Male;
  else if (gender == LS("female"))
    m_gender = Female;
  else if (gender == LS("ghost"))
    m_gender = Ghost;
  else if (gender == LS("bot"))
    m_gender = Bot;
  else
    m_gender = Unknown;

  if (m_gender > Female)
    return;

  if (color == LS("black"))
    m_gender += Black;
  else if (color == LS("gray"))
    m_gender += Gray;
  if (color == LS("green"))
    m_gender += Green;
  else if (color == LS("red"))
    m_gender += Red;
  else if (color == LS("white"))
    m_gender += White;
  else if (color == LS("yellow"))
    m_gender += Yellow;
  else if (color == LS("medical"))
    m_gender += Medical;
  else if (color == LS("nude"))
    m_gender += Nude;
  else if (color == LS("thief"))
    m_gender += Thief;
}
