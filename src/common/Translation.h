/* $Id: Translation.h 2458 2012-03-29 13:43:46Z IMPOMEZIA $
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

#ifndef TRANSLATION_H_
#define TRANSLATION_H_

#include <QMap>
#include <QObject>
#include <QStringList>

#include "schat.h"

class QTranslator;

class SCHAT_EXPORT Translation : public QObject
{
  Q_OBJECT

public:
  Translation(QObject *parent = 0);
  inline const QString& language() const        { return m_language; }
  inline const QString& name() const            { return m_name; }
  inline const QString& prefix() const          { return m_prefix; }
  inline const QStringList& search() const      { return m_search; }
  inline void setPrefix(const QString &prefix)  { m_prefix = prefix; }
  void addOther(const QString &name);
  void load(const QString &name);
  void setSearch(const QStringList &search);

private:
  void clear();
  void finalize();
  void loadOther(const QString &name);

  bool m_empty;                         ///< true если не было загружено ни одного перевода.
  QMap<QString, QTranslator*> m_others; ///< Префиксы дополнительных переводов.
  QString m_language;                   ///< Человеко-понятное переведённое имя языка.
  QString m_name;                       ///< Имя языка, например ru, en или auto.
  QString m_prefix;                     ///< Префикс файлов перевода.
  QStringList m_search;                 ///< Список директорий для поиска переводов.
  QTranslator *m_core;                  ///< Основной загрузчик переводов.
};

#endif /* TRANSLATION_H_ */
