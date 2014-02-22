// random.h

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

#pragma once

#include <QtGlobal>

#include "schat.h"

/**
 * Uses http://en.wikipedia.org/wiki/Xorshift
 */
class SCHAT_EXPORT PseudoRandom
{
public:
  PseudoRandom(qint32 seed);
  PseudoRandom(quint32 seed);
  PseudoRandom(qint64 seed);

  qint32 nextInt32();
  qint64 nextInt64();

  /**
   * @return a number between 0 and max
   */
  inline qint32 nextInt32(qint32 max) { return nextInt32() % max; }

private:
  qint32 _x;
  qint32 _y;
  qint32 _z;
  qint32 _w;
};

/**
 * More secure random numbers
 * Suitable for nonce/crypto
 * Slower than PseudoRandom, so only use when really need
 */
class SecureRandom
{
public:
  virtual ~SecureRandom() {}
  virtual qint64 nextInt64() = 0;
  static SecureRandom* create();
};
