/* $Id: Idle_mac.cpp 2817 2012-07-02 02:08:48Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
 * Copyright © 2003 Justin Karneges <justin@affinix.com> (from KVIrc source code)
 * Copyright © 2003 Stanford University <http://www.cocoabuilder.com/archive/cocoa/120261-system-idle-time.html#120354>
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

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <IOKit/IOKitLib.h>

IdlePlatform::IdlePlatform()
  : d(0)
{
}


IdlePlatform::~IdlePlatform()
{
}


bool IdlePlatform::init()
{
  if (secondsIdle() == -1)
    return false;

  return true;
}


int IdlePlatform::secondsIdle()
{
  static io_service_t macIOsrvc = NULL;
  CFTypeRef property;
  uint64_t idle_time = 0;

  if (macIOsrvc == 0) {
    mach_port_t master;
    IOMasterPort(MACH_PORT_NULL, &master);
    macIOsrvc = IOServiceGetMatchingService(master, IOServiceMatching("IOHIDSystem"));
  }

  property = IORegistryEntryCreateCFProperty(macIOsrvc, CFSTR("HIDIdleTime"), kCFAllocatorDefault, 0);
  CFNumberGetValue((CFNumberRef)property, kCFNumberSInt64Type, &idle_time);
  CFRelease(property);

  /* convert nanoseconds to seconds */
  return idle_time / 1000000000;
}
