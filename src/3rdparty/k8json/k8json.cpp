/* coded by Ketmar // Vampire Avalon (psyc://ketmar.no-ip.org/~Ketmar)
 * Understanding is not required. Only obedience.
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 */
#include <QStringList>

#include "k8json.h"

namespace K8JSON {

static const quint8 utf8Length[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0x00-0x0f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0x10-0x1f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0x20-0x2f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0x30-0x3f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0x40-0x4f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0x50-0x5f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0x60-0x6f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, //0x70-0x7f
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, //0x80-0x8f
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, //0x90-0x9f
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, //0xa0-0xaf
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, //0xb0-0xbf
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, //0xc0-0xcf  c0-c1: overlong encoding: start of a 2-byte sequence, but code point <= 127
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, //0xd0-0xdf
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3, //0xe0-0xef
  4,4,4,4,4,8,8,8,8,8,8,8,8,8,8,8  //0xf0-0xff
};


/*
 * check if given (const uchar *) represents valid UTF-8 sequence
 * NULL (or empty) s is not valid
 */
bool isValidUtf8 (const uchar *s, int maxLen, bool zeroInvalid) {
  if (!s) return false;
  if (maxLen < 1) return false;
  uchar ch = 0;
  const uchar *tmpS = s;
  while (maxLen > 0) {
    ch = *tmpS++; maxLen--;
    if (!ch) {
      if (maxLen != 0 && zeroInvalid) return false;
      break;
    }
    // ascii or utf-8
    quint8 t = utf8Length[ch];
    if (t&0x08) return false; // invalid utf-8 sequence
    if (t) {
      // utf-8
      if (maxLen < --t) return false; // invalid utf-8 sequence
      while (t--) {
        quint8 b = *tmpS++; maxLen--;
        if (utf8Length[b] != 9) return false; // invalid utf-8 sequence
      }
    }
  }
  return true;
}


QString quote(const QString &str) {
  int len = str.length();
  int c;
  QString res;
  res.reserve(len + 256);
  res += QLatin1Char('"');

  for (int f = 0; f < len; f++) {
    QChar ch(str[f]);
    ushort uc = ch.unicode();

    if (uc < 32) {
      // control char
      switch (uc) {
        case '\b': res += QLatin1String("\\b"); break;
        case '\f': res += QLatin1String("\\f"); break;
        case '\n': res += QLatin1String("\\n"); break;
        case '\r': res += QLatin1String("\\r"); break;
        case '\t': res += QLatin1String("\\t"); break;
        default:
          res += QLatin1String("\\u");
          for (c = 4; c > 0; c--) {
            ushort n = (uc>>12)&0x0f;
            n += '0' + (n > 9 ? 7 : 0);
            res += (uchar)n;
          }
          break;
      }
    } else {
      // normal char
      switch (uc) {
        case '"': res += QLatin1String("\\\""); break;
        case '\\': res += QLatin1String("\\\\"); break;
        default: res += ch; break;
      }
    }
  }

  res += QLatin1Char('"');
  return res;
}


/*
 * skip blanks and comments
 * return ptr to first non-blank char or 0 on error
 * 'maxLen' will be changed
 */
const uchar *skipBlanks (const uchar *s, int *maxLength) {
  if (!s) return 0;
  int maxLen = *maxLength;
  if (maxLen < 0) return 0;
  while (maxLen > 0) {
    // skip blanks
    uchar ch = *s++; maxLen--;
    if (ch <= ' ') continue;
    // skip comments
    if (ch == '/') {
      if (maxLen < 2) return 0;
      switch (*s) {
        case '/':
          while (maxLen > 0) {
            s++; maxLen--;
            if (s[-1] == '\n') break;
            if (maxLen < 1) return 0;
          }
          break;
        case '*':
          s++; maxLen--; // skip '*'
          while (maxLen > 0) {
            s++; maxLen--;
            if (s[-1] == '*' && s[0] == '/') {
              s++; maxLen--; // skip '/'
              break;
            }
            if (maxLen < 2) return 0;
          }
          break;
        default: return 0; // error
      }
      continue;
    }
    // it must be a token
    s--; maxLen++;
    break;
  }
  // done
  *maxLength = maxLen;
  return s;
}


//FIXME: table?
static inline bool isValidIdChar (const uchar ch) {
  return (
    ch == '$' || ch == '_' || ch >= 128 ||
    (ch >= '0' && ch <= '9') ||
    (ch >= 'A' && ch <= 'Z') ||
    (ch >= 'a' && ch <= 'z')
  );
}

/*
 * skip one record
 * the 'record' is either one full field ( field: val)
 * or one list/object.
 * return ptr to the first non-blank char after the record (or 0)
 * 'maxLen' will be changed
 */
const uchar *skipRec (const uchar *s, int *maxLength) {
  if (!s) return 0;
  int maxLen = *maxLength;
  if (maxLen < 0) return 0;
  int fieldNameSeen = 0;
  while (maxLen > 0) {
    // skip blanks
    if (!(s = skipBlanks(s, &maxLen))) return 0;
    if (!maxLen) break;
    uchar qch, ch = *s++; maxLen--;
    // fieldNameSeen<1: no field name was seen
    // fieldNameSeen=1: waiting for ':'
    // fieldNameSeen=2: field name was seen, ':' was seen too, waiting for value
    // fieldNameSeen=3: everything was seen, waiting for terminator
    if (ch == ':') {
      if (fieldNameSeen != 1) return 0; // wtf?
      fieldNameSeen++;
      continue;
    }
    // it must be a token, skip it
    bool again = false;
    switch (ch) {
      case '{': case '[':
        if (fieldNameSeen == 1) return 0; // waiting for delimiter; error
        fieldNameSeen = 3;
        // recursive skip
        qch = (ch=='{' ? '}' : ']'); // end char
        for (;;) {
          if (!(s = skipRec(s, &maxLen))) return 0;
          if (maxLen < 1) return 0; // no closing char
          ch = *s++; maxLen--;
          if (ch == ',') continue; // skip next field/value pair
          if (ch == qch) break; // end of the list or object
          return 0; // error!
        }
        break;
      case ']': case '}': case ',': // terminator
        if (fieldNameSeen != 3) return 0; // incomplete field
        s--; maxLen++; // back to this char
        break;
      case '"': case '\x27': // string
        if (fieldNameSeen == 1 || fieldNameSeen > 2) return 0; // no delimiter
        fieldNameSeen++;
        qch = ch;
        while (*s && maxLen > 0) {
          ch = *s++; maxLen--;
          if (ch == qch) { s--; maxLen++; break; }
          if (ch != '\\') continue;
          if (maxLen < 2) return 0; // char and quote
          ch = *s++; maxLen--;
          switch (ch) {
            case 'u':
              if (maxLen < 5) return 0;
              if (s[0] == qch || s[0] == '\\' || s[1] == qch || s[1] == '\\') return 0;
              s += 2; maxLen -= 2;
              // fallthru
            case 'x':
              if (maxLen < 3) return 0;
              if (s[0] == qch || s[0] == '\\' || s[1] == qch || s[1] == '\\') return 0;
              s += 2; maxLen -= 2;
              break;
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
              if (maxLen < 4) return 0;
              if (s[0] == qch || s[0] == '\\' || s[1] == qch || s[1] == '\\' || s[2] == qch || s[2] == '\\') return 0;
              s += 3; maxLen -= 3;
              break;
            default: ; // escaped char already skiped
          }
        }
        if (maxLen < 1 || *s != qch) return 0; // error
        s++; maxLen--; // skip quote
        again = true;
        break;
      default: // we can check for punctuation here, but i'm too lazy to do it
        if (fieldNameSeen == 1 || fieldNameSeen > 2) return 0; // no delimiter
        fieldNameSeen++;
        if (isValidIdChar(ch)) {
          // good token, skip it
          again = true; // just a token, skip it and go on
          // check for valid utf8?
          while (*s && maxLen > 0) {
            ch = *s++; maxLen--;
            if (ch != '.' && !isValidIdChar(ch)) {
              s--; maxLen++;
              break;
            }
          }
        } else return 0; // error
    }
    if (!again) break;
  }
  if (fieldNameSeen != 3) return 0;
  // skip blanks
  if (!(s = skipBlanks(s, &maxLen))) return 0;
  // done
  *maxLength = maxLen;
  return s;
}


/*
 * parse json-quoted string. a relaxed parser, it allows "'"-quoted strings,
 * whereas json standard does not. also \x and \nnn are allowed.
 * return position after the string or 0
 * 's' should point to the quote char on entry
 */
static const uchar *parseString (QString &str, const uchar *s, int *maxLength) {
  if (!s) return 0;
  int maxLen = *maxLength;
  if (maxLen < 2) return 0;
  uchar ch = 0, qch = *s++; maxLen--;
  if (qch != '"' && qch != '\x27') return 0;
  // calc string length and check string for correctness
  int strLen = 0, tmpLen = maxLen;
  const uchar *tmpS = s;
  while (tmpLen > 0) {
    ch = *tmpS++; tmpLen--; strLen++;
    if (ch == qch) break;
    if (ch != '\\') {
      // ascii or utf-8
      quint8 t = utf8Length[ch];
      if (t&0x08) return 0; // invalid utf-8 sequence
      if (t) {
        // utf-8
        if (tmpLen < t) return 0; // invalid utf-8 sequence
        while (--t) {
          quint8 b = *tmpS++; tmpLen--;
          if (utf8Length[b] != 9) return 0; // invalid utf-8 sequence
        }
      }
      continue;
    }
    // escape sequence
    ch = *tmpS++; tmpLen--; //!strLen++;
    if (tmpLen < 2) return 0;
    int hlen = 0;
    switch (ch) {
      case 'u': hlen = 4;
      case 'x':
        if (!hlen) hlen = 2;
        if (tmpLen < hlen+1) return 0;
        while (hlen-- > 0) {
          ch = *tmpS++; tmpLen--;
          if (ch >= 'a') ch -= 32;
          if (!(ch >= '0' && ch <= '9') && !(ch >= 'A' && ch <= 'F')) return 0;
        }
        hlen = 0;
        break;
      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': // octal char
        if (tmpLen < 3) return 0;
        for (hlen = 2; hlen > 0; hlen--) {
          ch = *tmpS++; tmpLen--;
          if (ch < '0' || ch > '7') return 0;
        }
        break;
      case '"': case '\x27': ch = 0; break;
      default: ; // one escaped char; will be checked later
    }
  }
  if (ch != qch) return 0; // no terminating quote
  //
  str.reserve(str.length()+strLen+1);
  ch = 0;
  while (maxLen > 0) {
    ch = *s++; maxLen--;
    if (ch == qch) break;
    if (ch != '\\') {
      // ascii or utf-8
      quint8 t = utf8Length[ch];
      if (!t) str.append(QLatin1Char(ch)); // ascii
      else {
        // utf-8
        int u = 0; s--; maxLen++;
        while (t--) {
          quint8 b = *s++; maxLen--;
          u = (u<<6)+(b&0x3f);
        }
        if (u > 0x10ffff) u &= 0xffff;
        if ((u >= 0xd800 && u <= 0xdfff) || // utf16/utf32 surrogates
            (u >= 0xfdd0 && u <= 0xfdef) || // just for fun
            (u >= 0xfffe && u <= 0xffff)) continue; // bad unicode, skip it
        QChar zch(u);
        str.append(zch);
      }
      continue;
    }
    ch = *s++; maxLen--; // at least one char left here
    int uu = 0; int escCLen = 0;
    switch (ch) {
      case 'u': // unicode char, 4 hex digits
        escCLen = 4;
        // fallthru
      case 'x': { // ascii char, 2 hex digits
        if (!escCLen) escCLen = 2;
        while (escCLen-- > 0) {
          ch = *s++; maxLen--;
          if (ch >= 'a') ch -= 32;
          uu = uu*16+ch-'0';
          if (ch >= 'A'/* && ch <= 'F'*/) uu -= 7;
        }
        if (uu > 0x10ffff) uu &= 0xffff;
        if ((uu >= 0xd800 && uu <= 0xdfff) || // utf16/utf32 surrogates
            (uu >= 0xfdd0 && uu <= 0xfdef) || // just for fun
            (uu >= 0xfffe && uu <= 0xffff)) uu = -1; // bad unicode, skip it
        if (uu >= 0) {
          QChar zch(uu);
          str.append(zch);
        }
        } break;
      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': { // octal char
        s--; maxLen++;
        uu = 0;
        for (int f = 3; f > 0; f--) {
          ch = *s++; maxLen--;
          uu = uu*8+ch-'0';
        }
        QChar zch(uu);
        str.append(zch);
        } break;
      case '\\': str.append('\\'); break;
      case '/': str.append('/'); break;
      case 'b': str.append('\b'); break;
      case 'f': str.append('\f'); break;
      case 'n': str.append('\n'); break;
      case 'r': str.append('\r'); break;
      case 't': str.append('\t'); break;
      case '"': case '\x27': str.append(ch); break;
      default:
        // non-standard!
        if (ch != '\t' && ch != '\r' && ch != '\n') return 0; // all other chars are BAD
        str.append(ch);
    }
  }
  if (ch != qch) return 0;
  *maxLength = maxLen;
  return s;
}


/*
 * parse identifier
 */
static const uchar *parseId (QString &str, const uchar *s, int *maxLength) {
  if (!s) return 0;
  int maxLen = *maxLength;
  if (maxLen < 1) return 0;
  uchar ch = 0;
  // calc string length and check string for correctness
  int strLen = 0, tmpLen = maxLen;
  const uchar *tmpS = s;
  while (tmpLen > 0) {
    ch = *tmpS++;
    if (!isValidIdChar(ch)) {
      if (!strLen) return 0;
      break;
    }
    tmpLen--; strLen++;
    // ascii or utf-8
    quint8 t = utf8Length[ch];
    if (t&0x08) return 0; // invalid utf-8 sequence
    if (t) {
      // utf-8
      if (tmpLen < t) return 0; // invalid utf-8 sequence
      while (--t) {
        quint8 b = *tmpS++; tmpLen--;
        if (utf8Length[b] != 9) return 0; // invalid utf-8 sequence
      }
    }
    continue;
  }
/*
  str = "true";
  while (isValidIdChar(*s)) { s++; (*maxLength)--; }
  return s;
*/
  //
  str.reserve(str.length()+strLen+1);
  ch = 0;
  while (maxLen > 0) {
    ch = *s++; maxLen--;
    if (!isValidIdChar(ch)) { s--; maxLen++; break; }
    // ascii or utf-8
    quint8 t = utf8Length[ch];
    if (!t) str.append(ch); // ascii
    else {
      // utf-8
      int u = 0; s--; maxLen++;
      while (t--) {
        quint8 ch = *s++; maxLen--;
        u = (u<<6)+(ch&0x3f);
      }
      if (u > 0x10ffff) u &= 0xffff;
      if ((u >= 0xd800 && u <= 0xdfff) || // utf16/utf32 surrogates
          (u >= 0xfdd0 && u <= 0xfdef) || // just for fun
          (u >= 0xfffe && u <= 0xffff)) continue; // bad unicode, skip it
      QChar zch(u);
      str.append(zch);
    }
    continue;
  }
  *maxLength = maxLen;
  return s;
}


/*
 * parse number
 */
//TODO: parse 0x...
static const uchar *parseNumber (QVariant &num, const uchar *s, int *maxLength) {
  if (!s) return 0;
  int maxLen = *maxLength;
  if (maxLen < 1) return 0;
  uchar ch = *s++; maxLen--;
  // check for negative number
  bool negative = false, fr = false;
  double fnum = 0.0;
  switch (ch) {
    case '-':
      if (maxLen < 1) return 0;
      ch = *s++; maxLen--;
      negative = true;
      break;
    case '+':
      if (maxLen < 1) return 0;
      ch = *s++; maxLen--;
      break;
    default: ;
  }
  if (ch != '.' && (ch < '0' || ch > '9')) return 0; // invalid integer part, no fraction part
  if (ch == '0' && *maxLength > 0 && *s == 'x') {
    // hex number
    s++; (*maxLength)--; // skip 'x'
    bool wasDigit = false;
    for (;;) {
      if (*maxLength < 1) break;
      uchar ch = *s++; (*maxLength)--;
      //if (ch >= 'a' && ch <= 'f') ch -= 32;
      if (ch >= '0' && ch <= '9') {
        fnum *= 16; fnum += ch-'0';
      } else if (ch >= 'A' && ch <= 'F') {
        fnum *= 16; fnum += ch-'A'+10;
      } else if (ch >= 'a' && ch <= 'f') {
        fnum *= 16; fnum += ch-'a'+10;
      } else break;
      wasDigit = true;
    }
    if (!wasDigit) return 0;
    goto backanddone;
  }
  // parse integer part
  while (ch >= '0' && ch <= '9') {
    ch -= '0';
    fnum = fnum*10+ch;
    if (!maxLen) goto done;
    ch = *s++; maxLen--;
  }
  // check for fractional part
  if (ch == '.') {
    // parse fractional part
    if (maxLen < 1) return 0;
    ch = *s++; maxLen--;
    double frac = 0.1; fr = true;
    if (ch < '0' || ch > '9') return 0; // invalid fractional part
    while (ch >= '0' && ch <= '9') {
      ch -= '0';
      fnum += frac*ch;
      if (!maxLen) goto done;
      frac /= 10;
      ch = *s++; maxLen--;
    }
  }
  // check for exp part
  if (ch == 'e' || ch == 'E') {
    if (maxLen < 1) return 0;
    // check for exp sign
    bool expNeg = false;
    ch = *s++; maxLen--;
    if (ch == '+' || ch == '-') {
      if (maxLen < 1) return 0;
      expNeg = (ch == '-');
      ch = *s++; maxLen--;
    }
    // check for exp digits
    if (ch < '0' || ch > '9') return 0; // invalid exp
    quint32 exp = 0; // 64? %-)
    while (ch >= '0' && ch <= '9') {
      exp = exp*10+ch-'0';
      if (!maxLen) { s++; maxLen--; break; }
      ch = *s++; maxLen--;
    }
    while (exp--) {
      if (expNeg) fnum /= 10; else fnum *= 10;
    }
    if (expNeg && !fr) {
      if (fnum > 2147483647.0 || ((qint64)fnum)*1.0 != fnum) fr = true;
    }
  }
backanddone:
  s--; maxLen++;
done:
//  if (!fr && fnum > 2147483647.0) fr = true;
  if (negative) fnum = -fnum;
  if (fr) num = fnum; else num = (qint64)fnum;
  *maxLength = maxLen;
  return s;
}


static const QString sTrue("true");
static const QString sFalse("false");
static const QString sNull("null");


/*
 * parse field value
 * return ptr to the first non-blank char after the value (or 0)
 * 'maxLen' will be changed
 */
const uchar *parseValue (QVariant &fvalue, const uchar *s, int *maxLength) {
  fvalue.clear();
  if (!(s = skipBlanks(s, maxLength))) return 0;
  if (*maxLength < 1) return 0;
  switch (*s) {
    case '-': case '+': case '.': // '+' and '.' are not in specs!
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      // number
      if (!(s = parseNumber(fvalue, s, maxLength))) return 0;
      break;
    case 't': // true?
      if (*maxLength < 4) return 0;
      if (s[1] != 'r' || s[2] != 'u' || s[3] != 'e') return 0;
      if (*maxLength > 4 && isValidIdChar(s[4])) return 0;
      s += 4; (*maxLength) -= 4;
      fvalue = true;
      break;
    case 'f': // false?
      if (*maxLength < 5) return 0;
      if (s[1] != 'a' || s[2] != 'l' || s[3] != 's' || s[4] != 'e') return 0;
      if (*maxLength > 5 && isValidIdChar(s[5])) return 0;
      s += 5; (*maxLength) -= 5;
      fvalue = false;
      break;
    case 'n': // null?
      if (*maxLength < 4) return 0;
      if (s[1] != 'u' || s[2] != 'l' || s[3] != 'l') return 0;
      if (*maxLength > 4 && isValidIdChar(s[4])) return 0;
      s += 4; (*maxLength) -= 4;
      // fvalue is null already
      break;
    case '"': case '\x27': {
      // string
      QString tmp;
      if (!(s = parseString(tmp, s, maxLength))) return 0;
      fvalue = tmp;
      }
      break;
    case '{': case '[':
      // object or list
      if (!(s = parseRecord(fvalue, s, maxLength))) return 0;
      break;
    default: // unknown
      return 0;
  }
  if (!(s = skipBlanks(s, maxLength))) return 0;
  return s;
}


/*
 * parse one field (f-v pair)
 * return ptr to the first non-blank char after the record (or 0)
 * 'maxLen' will be changed
 */
const uchar *parseField (QString &fname, QVariant &fvalue, const uchar *s, int *maxLength) {
  if (!s) return 0;
  //int maxLen = *maxLength;
  fname.clear();
  fvalue.clear();
  if (!(s = skipBlanks(s, maxLength))) return 0;
  if (*maxLength < 1) return 0;
  uchar ch = *s;
  // field name
  if (isValidIdChar(ch)) {
    // id
    if (!(s = parseId(fname, s, maxLength))) return 0;
  } else if (ch == '"' || ch == '\x27') {
    // string
    if (!(s = parseString(fname, s, maxLength))) return 0;
  }
  // ':'
  if (!(s = skipBlanks(s, maxLength))) return 0;
  if (*maxLength < 2 || *s != ':') return 0;
  s++; (*maxLength)--;
  // field value
  return parseValue(fvalue, s, maxLength);
}


/*
 * parse one record (list or object)
 * return ptr to the first non-blank char after the record (or 0)
 * 'maxLen' will be changed
 */
const uchar *parseRecord (QVariant &res, const uchar *s, int *maxLength) {
  if (!s) return 0;
  //int maxLen = *maxLength;
  res.clear();
  if (!(s = skipBlanks(s, maxLength))) return 0;
  if (*maxLength < 1) return 0;
  // field name or list/object start
  QString str; QVariant val;
  uchar ch = *s;
  bool isList = false;
  switch (ch) {
    case '[': isList = true; // list, fallthru
    case '{': { // object
      if (*maxLength < 2) return 0;
      uchar ech = isList ? ']' : '}';
      s++; (*maxLength)--;
      QVariantMap obj; QVariantList lst;
      if (*s == ech) {
          s++;
          (*maxLength)--;
      } else for (;;) {
        if (isList) {
          // list, only values
          if (!(s = parseValue(val, s, maxLength))) return 0;
          lst << val;
        } else {
          // object, full fields
          if (!(s = parseField(str, val, s, maxLength))) return 0;
          obj[str] = val;
        }
        if (*maxLength > 0) {
          bool wasComma = false;
          // skip commas
          while (true) {
            if (!(s = skipBlanks(s, maxLength))) return 0;
            if (*maxLength < 1) { ch = '\0'; wasComma = false; break; }
            ch = *s;
            if (ch == ech) { s++; (*maxLength)--; break; }
            if (ch != ',') break;
            s++; (*maxLength)--;
            wasComma = true;
          }
          if (ch == ech) break; // end of the object/list
          if (wasComma) continue;
          // else error
        }
        // error
        s = 0;
        break;
      }
      if (isList) res = lst; else res = obj;
      return s;
      } // it will never comes here
    default: ;
  }
  if (!(s = parseField(str, val, s, maxLength))) return 0;
  QVariantMap obj;
  obj[str] = val;
  res = obj;
  return s;
}


bool generate(QByteArray &res, const QVariant &val, int indent) {
  switch (val.type()) {
    case QVariant::Invalid:   res += QLatin1String("null"); break;
    case QVariant::Bool:      res += (val.toBool() ? QLatin1String("true") : QLatin1String("false")); break;
    case QVariant::Char:      res += quote(QString(val.toChar())).toUtf8(); break;
    case QVariant::Double:    res += QByteArray::number(val.toDouble()); break; //CHECKME: is '.' always '.'?
    case QVariant::Int:       res += QByteArray::number(val.toInt()); break;
    case QVariant::LongLong:  res += QByteArray::number(val.toLongLong()); break;
    case QVariant::UInt:      res += QByteArray::number(val.toUInt()); break;
    case QVariant::ULongLong: res += QByteArray::number(val.toULongLong()); break;
    case QVariant::String:    res += quote(val.toString()).toUtf8(); break;
    case QVariant::ByteArray: res += quote(val.toByteArray()); break;

    case QVariant::Map: {
      res += '{';
      indent++;
      bool comma = false;
      QVariantMap m(val.toMap());
      QMapIterator<QString, QVariant> i(m);
      while (i.hasNext()) {
        i.next();
        if (!comma) {
          res += '\n';
          comma = true;
        }
        else
          res += QLatin1String(",\n");

        for (int c = indent; c > 0; c--)
          res += ' ';

        res += quote(i.key()).toUtf8();
        res += QLatin1String(": ");
        generate(res, i.value(), indent);
      }
      indent--;
      if (comma) {
        res += '\n';
        for (int c = indent; c > 0; c--)
          res += ' ';
      }
      res += '}';
      indent--;
    }
    break;

    case QVariant::Hash: {
      res += '{';
      indent++;
      bool comma = false;
      QVariantHash m(val.toHash());
      QHashIterator<QString, QVariant> i(m);
      while (i.hasNext()) {
        i.next();
        if (!comma) {
          res += '\n';
          comma = true;
        }
        else
          res += QLatin1String(",\n");

        for (int c = indent; c > 0; c--)
          res += ' ';

        res += quote(i.key()).toUtf8();
        res += QLatin1String(": ");
        generate(res, i.value(), indent);
      }

      indent--;
      if (comma) {
        res += '\n';
        for (int c = indent; c > 0; c--)
          res += ' ';
      }
      res += '}';
      indent--;
    }
    break;

    case QVariant::List: {
      res += '[';
      indent++;
      bool comma = false;
      QVariantList m(val.toList());
      foreach (const QVariant &v, m) {
        if (!comma) {
          res += '\n';
          comma = true;
        }
        else
          res += QLatin1String(",\n");

        for (int c = indent; c > 0; c--)
          res += ' ';

        generate(res, v, indent);
      }

      indent--;
      if (comma) {
        res += '\n';
        for (int c = indent; c > 0; c--)
          res += ' ';
      }
      res += ']';
      indent--;
    }
    break;

    case QVariant::StringList: {
      res += '[';
      indent++;
      bool comma = false;
      QStringList m(val.toStringList());
      foreach (const QString &v, m) {
        if (!comma) {
          res += '\n';
          comma = true;
        }
        else
          res += QLatin1String(",\n");

        for (int c = indent; c > 0; c--)
          res += ' ';
        res += quote(v).toUtf8();
      }

      indent--;
      if (comma) {
        res += '\n';
        for (int c = indent; c > 0; c--)
          res += ' ';
      }
      res += ']';
      indent--;
    }
    break;

    default:
      if (val.canConvert(QVariant::String))
        generate(res, val.toString(), indent);
      else
        res += QLatin1String("null");
  }
  return true;
}


}
