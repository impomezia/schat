/* $Id: ObjectId.cpp 3700 2013-06-22 00:23:39Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 Alexander Sedov <support@schat.me>
 * Copyright © 2009 10gen Inc.
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

#ifdef _WIN32
# include <qt_windows.h>
#else
# include <sys/types.h>
# include <unistd.h>
#endif

#include <time.h>

#include <QScopedPointer>

#include "id/ObjectId.h"
#include "SecureRandom.h"

// machine # before folding in the process id
ObjectId::MachineAndPid ObjectId::ourMachine;

// after folding in the process id
ObjectId::MachineAndPid ObjectId::ourMachineAndPid = ObjectId::genMachineAndPid();


ObjectId &ObjectId::init()
{
  static QAtomicInt inc = static_cast<unsigned>(QScopedPointer<SecureRandom>(SecureRandom::create())->nextInt64());

  {
    unsigned t = (unsigned) ::time(0);
    uchar *T = (uchar *) &t;
    _time[0] = T[3]; // big endian order because we use memcmp() to compare OID's
    _time[1] = T[2];
    _time[2] = T[1];
    _time[3] = T[0];
  }

  _machineAndPid = ourMachineAndPid;

  {
    const int new_inc = inc.fetchAndAddOrdered(1);
    uchar *T = (uchar *) &new_inc;
    _inc[0] = T[2];
    _inc[1] = T[1];
    _inc[2] = T[0];
  }

  return *this;
}


int ObjectId::time() const
{
  int time;
  char* T = (char *) &time;
  T[0] = m_data[3];
  T[1] = m_data[2];
  T[2] = m_data[1];
  T[3] = m_data[0];
  return time;
}


void ObjectId::justForked()
{
  MachineAndPid x = ourMachine;
  // we let the random # for machine go into all 5 bytes of MachineAndPid, and then
  // xor in the pid into _pid.  this reduces the probability of collisions.
  foldInPid(x);
  ourMachineAndPid = genMachineAndPid();
  ourMachineAndPid = x;
}


unsigned ObjectId::ourPid()
{
# ifdef _WIN32
  return static_cast<unsigned>( GetCurrentProcessId() );
# else
  return static_cast<unsigned>( getpid() );
# endif
}


void ObjectId::foldInPid(MachineAndPid& x)
{
  unsigned p = ourPid();
  x._pid ^= (ushort) p;
  // when the pid is greater than 16 bits, let the high bits modulate the machine id field.
  ushort& rest = (ushort &) x._machineNumber[1];
  rest ^= p >> 16;
}


ObjectId::MachineAndPid ObjectId::genMachineAndPid()
{
  qint64 n = QScopedPointer<SecureRandom>(SecureRandom::create())->nextInt64();
  ObjectId::MachineAndPid x = ourMachine = reinterpret_cast<ObjectId::MachineAndPid&>(n);
  foldInPid(x);
  return x;
}
