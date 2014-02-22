/* $Id: Idle_x11.cpp 3309 2012-12-09 15:15:50Z IMPOMEZIA $
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

#include "Idle.h"

#if defined(SCHAT_NO_XSS)
IdlePlatform::IdlePlatform() : d(0) {}
IdlePlatform::~IdlePlatform() {}
bool IdlePlatform::init() { return false; }
int IdlePlatform::secondsIdle() { return 0; }
#else

#include <QApplication>
#include <QDesktopWidget>
#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

class IdlePlatform::Private {
public:
  Private() {}

  XScreenSaverInfo *ss_info;
};


IdlePlatform::IdlePlatform()
  : d(0)
{
  d = new Private;
  d->ss_info = 0;
}


IdlePlatform::~IdlePlatform()
{
  if (d->ss_info)
    XFree(d->ss_info);

  delete d;
}


bool IdlePlatform::init()
{
  if (d->ss_info)
    return true;

  int event_base, error_base;
  if (XScreenSaverQueryExtension(QX11Info::display(), &event_base, &error_base)) {
    d->ss_info = XScreenSaverAllocInfo();
    return true;
  }

  return false;
}


int IdlePlatform::secondsIdle()
{
  if (!d->ss_info)
    return 0;

  if (!XScreenSaverQueryInfo(QX11Info::display(), QX11Info::appRootWindow(), d->ss_info))
    return 0;

  return d->ss_info->idle / 1000;
}
#endif
