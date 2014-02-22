/* $Id: JSON.cpp 2762 2012-06-11 06:37:18Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#include <QStringList>

#include "k8json/k8json.h"
#include "JSON.h"

/*!
 * Генерация JSON.
 *
 * \param data      Исходные данные.
 * \param formatted \b true если необходимо форматированный результат.
 *
 * \return Результат генерации.
 */
QByteArray JSON::generate(const QVariant &data, bool formatted)
{
  QByteArray res;
  if (formatted)
    K8JSON::generate(res, data);
  else
    gen(res, data);

  if (res == "{}")
    return QByteArray();

  return res;
}


/*!
 * Формирование корректной строки для использования внутри JSON.
 */
QString JSON::quote(const QString &text)
{
  return K8JSON::quote(text);
}


/*!
 * Разбор JSON данных.
 */
QVariant JSON::parse(const QByteArray &data)
{
  QVariant res;
  if (data.isEmpty())
    return res;

  int size = data.size();
  K8JSON::parseRecord(res, reinterpret_cast<const uchar *>(data.constData()), &size);
  return res;
}


void JSON::gen(QByteArray &res, const QVariant &val)
{
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
      bool comma = false;
      QVariantMap m(val.toMap());
      QMapIterator<QString, QVariant> i(m);
      while (i.hasNext()) {
        i.next();
        if (comma)
          res += ',';
        else
          comma = true;

        res += quote(i.key()).toUtf8();
        res += ':';
        gen(res, i.value());
      }
      res += '}';
    }
    break;

    case QVariant::Hash: {
      res += '{';
      bool comma = false;
      QVariantHash m(val.toHash());
      QHashIterator<QString, QVariant> i(m);
      while (i.hasNext()) {
        i.next();
        if (comma)
          res += ',';
        else
          comma = true;

        res += quote(i.key()).toUtf8();
        res += ':';
        gen(res, i.value());
      }
      res += '}';
    }
    break;

    case QVariant::List: {
      res += '[';
      bool comma = false;
      QVariantList m(val.toList());
      foreach (const QVariant &v, m) {
        if (comma)
          res += ',';
        else
          comma = true;

        gen(res, v);
      }

      res += ']';
    }
    break;

    case QVariant::StringList: {
      res += '[';
      bool comma = false;

      QStringList m(val.toStringList());
      foreach (const QString &v, m) {
        if (comma)
          res += ',';
        else
          comma = true;

        res += quote(v).toUtf8();
      }
      res += ']';
    }
    break;

    default:
      if (val.canConvert(QVariant::String))
        gen(res, val.toString());
      else
        res += QLatin1String("null");
  }
}
