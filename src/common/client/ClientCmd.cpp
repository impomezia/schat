/* $Id: ClientCmd.cpp 1542 2011-07-18 11:01:45Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2011 IMPOMEZIA <schat@impomezia.com>
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

#include "client/ClientCmd.h"

ClientCmd::ClientCmd(const QString &text)
  : m_isBody(false)
  , m_isValid(false)
{
  if (text.isEmpty())
    return;

  m_isValid = true;

  int index = text.indexOf(QLatin1String(" "));
  if (index == -1) {
    m_command = text;
    return;
  }

  m_command = text.left(index);
  m_body = text.mid(index + 1);
  if (!m_body.isEmpty())
    m_isBody = true;
}
