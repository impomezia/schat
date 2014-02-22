// random.cpp

/*    Copyright 2012 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "id/SecureRandom.h"

#if defined(_WIN32)
# include <errno.h>
# include <windows.h>
#else
# include <cstdlib>
# include <fstream>
#endif

#if defined(Q_OS_MAC)
# include <iostream>
#endif

#if !defined(_MSC_VER)
# include <time.h>
#endif

    // ---- PseudoRandom  -----

qint32 PseudoRandom::nextInt32()
{
  qint32 t = _x ^ (_x << 11);
  _x = _y;
  _y = _z;
  _z = _w;
  return _w = _w ^ (_w >> 19) ^ (t ^ (t >> 8));
}

namespace {
  const qint32 default_y = 362436069;
  const qint32 default_z = 521288629;
  const qint32 default_w = 88675123;
}

PseudoRandom::PseudoRandom(qint32 seed)
{
  _x = seed;
  _y = default_y;
  _z = default_z;
  _w = default_w;
}

PseudoRandom::PseudoRandom(quint32 seed)
{
  _x = static_cast<qint32> (seed);
  _y = default_y;
  _z = default_z;
  _w = default_w;
}

PseudoRandom::PseudoRandom(qint64 seed)
{
  qint32 high = seed >> 32;
  qint32 low = seed & 0xFFFFFFFF;

  _x = high ^ low;
  _y = default_y;
  _z = default_z;
  _w = default_w;
}

qint64 PseudoRandom::nextInt64()
{
  qint64 a = nextInt32();
  qint64 b = nextInt32();
  return (a << 32) | b;
}


// --- SecureRandom ----

#if defined(_WIN32)
# if defined(_MSC_VER)
  class WinSecureRandom : public SecureRandom
  {
  qint64 nextInt64()
  {
    quint32 a, b;
    if ( rand_s(&a) )
      abort();

    if ( rand_s(&b) )
      abort();

    return ( static_cast<qint64>(a) << 32 ) | b;
  }
};

SecureRandom* SecureRandom::create()
{
  return new WinSecureRandom();
}
# else
class PseudoSecureRandom : public SecureRandom
{
  qint64 nextInt64()
  {
    if (!m_init) {
      m_init = true;
      srand(time(0));
    }

    return PseudoRandom((qint32) time(0) ^ (rand() * rand())).nextInt64();
  }

private:
  static bool m_init;
};

bool PseudoSecureRandom::m_init = false;

SecureRandom* SecureRandom::create()
{
  return new PseudoSecureRandom();
}
# endif

#elif defined(__linux__) || defined(__sunos__) || defined(__APPLE__)

class InputStreamSecureRandom : public SecureRandom
{
public:
  InputStreamSecureRandom( const char* fn )
  {
    _in = new std::ifstream( fn, std::ios::binary | std::ios::in );
    if ( !_in->is_open() )
      abort();
  }

  ~InputStreamSecureRandom()
  {
    delete _in;
  }

  qint64 nextInt64()
  {
    qint64 r;
    _in->read( reinterpret_cast<char*>( &r ), sizeof(r) );
    if ( _in->fail() )
      abort();

    return r;
  }

private:
  std::ifstream* _in;
};

SecureRandom* SecureRandom::create()
{
  return new InputStreamSecureRandom( "/dev/urandom" );
}

#else
class SRandSecureRandom: public SecureRandom
{
public:
  SRandSecureRandom()
  {
    srandomdev();
  }

  qint64 nextInt64()
  {
    long a, b;
    a = random();
    b = random();
    return (static_cast<qint64> (a) << 32) | b;
  }
};

SecureRandom* SecureRandom::create()
{
  return new SRandSecureRandom();
}


#endif
