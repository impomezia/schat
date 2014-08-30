/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

#include "sglobal.h"
#include "ui/AuthIcon.h"

AuthIcon *AuthIcon::m_self = 0;

AuthIcon::AuthIcon()
{
  m_self = this;
  m_layout = QPixmap(LS(":/images/providers.png"));
}


QIcon AuthIcon::icon(const QString &provider)
{
  return QIcon(pixmap(provider));
}


QPixmap AuthIcon::pixmap(const QString &provider)
{
  if (!m_self)
    return QPixmap();

  int x = -1;
  if (provider == LS("google"))
    x = 16;
  else if (provider == LS("yandex"))
    x = 32;
  else if (provider == LS("facebook"))
    x = 48;
  else if (provider == LS("vkontakte"))
    x = 64;
  else if (provider == LS("mail_ru"))
    x = 80;
  else if (provider == LS("odnoklassniki"))
    x = 96;
  else if (provider == LS("live"))
    x = 112;
  else if (provider == LS("github"))
    x = 128;
  else if (provider == LS("stackoverflow"))
    x = 144;
  else if (provider == LS("simpleid"))
    x = 160;

  if (x == -1)
    return QPixmap();

  return m_self->m_layout.copy(x, 0, 16, 16);
}
