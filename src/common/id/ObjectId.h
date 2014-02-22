/* $Id: ObjectId.h 3719 2013-07-01 19:13:52Z IMPOMEZIA $
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

#ifndef OBJECTID_H_
#define OBJECTID_H_

#include <string.h>

#include <QByteArray>

#include "schat.h"

#pragma pack(1)
class SCHAT_EXPORT ObjectId
{
public:
  enum {
    kOIDSize = 12,
    kEncodedSize = 20
  };

  inline ObjectId() : a(0), b(0) {}
  inline explicit ObjectId(const uchar (&arr)[kOIDSize]) {
    memcpy(m_data, arr, kOIDSize);
  }

  inline bool isNull() const                          { return !a && !b; }
  inline const QByteArray byteArray() const           { return QByteArray::fromRawData(reinterpret_cast<const char *>(m_data), kOIDSize); }
  inline const uchar *data() const                    { return m_data; }
  inline static ObjectId gen()                        { return ObjectId().init(); }
  inline void clear()                                 { a = 0; b = 0; }

  inline bool operator!=(const ObjectId& other) const { return a != other.a || b != other.b; }
  inline bool operator<(const ObjectId& other) const  { return memcmp(m_data , other.m_data , kOIDSize ) < 0; }
  inline bool operator==(const ObjectId& other) const { return a == other.a && b == other.b; }

  ObjectId &init();
  int time() const;

  /** call this after a fork to update the process id */
  static void justForked();

private:
  struct MachineAndPid {
    uchar _machineNumber[3];
    ushort _pid;
    inline bool operator!=(const ObjectId::MachineAndPid& rhs) const {
      return _pid != rhs._pid || _machineNumber != rhs._machineNumber;
    }
  };

  union {
    struct {
      // 12 bytes total
      uchar _time[4];
      MachineAndPid _machineAndPid;
      uchar _inc[3];
    };
    struct {
      long long a;
      unsigned b;
    };

    uchar m_data[kOIDSize];
  };

  static MachineAndPid ourMachine;
  static MachineAndPid ourMachineAndPid;

  static unsigned ourPid();
  static void foldInPid(MachineAndPid& x);
  static MachineAndPid genMachineAndPid();
};
#pragma pack()

#endif /* OBJECTID_H_ */
