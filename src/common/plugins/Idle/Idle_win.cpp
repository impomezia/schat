/* $Id: Idle_win.cpp 2817 2012-07-02 02:08:48Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
 * Copyright © 2003 Justin Karneges <justin@affinix.com> (from KVIrc source code)
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

#include <qt_windows.h>

#include "Idle.h"

IdlePlatform::IdlePlatform()
  : d(0)
{
}


IdlePlatform::~IdlePlatform()
{
}


bool IdlePlatform::init()
{
  return true;
}


int IdlePlatform::secondsIdle()
{
  DWORD result = 0;

  LASTINPUTINFO lii;
  memset(&lii, 0, sizeof(lii));
  lii.cbSize = sizeof(lii);
  if (GetLastInputInfo(&lii))
    result = lii.dwTime;

  return (GetTickCount() - result ) / 1000;
}
