/**
 * \file crc8.h
 * Functions and types for CRC checks.
 *
 * Generated on Thu Oct  2 15:57:54 2014,
 * by pycrc v0.8.1, http://www.tty1.net/pycrc/
 * using the configuration:
 *    Width        = 8
 *    Poly         = 0x07
 *    XorIn        = 0x00
 *    ReflectIn    = False
 *    XorOut       = 0x00
 *    ReflectOut   = False
 *    Algorithm    = table-driven
 *****************************************************************************/
#ifndef __CRC8_H__
#define __CRC8_H__

#include <QtGlobal>

quint8 crc8(const char *data, uint len);

#endif      /* __CRC8_H__ */
