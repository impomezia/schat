/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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
#include "interfaces/ITranslation.h"

class QTranslator;

class SCHAT_EXPORT Translation : public QObject, public ITranslation
{
  Q_OBJECT

public:
  Translation(QObject *parent = 0);
  inline const QString& language() const        { return m_language; }
  QString name() const Q_DECL_OVERRIDE          { return m_name; }
  inline const QString& prefix() const          { return m_prefix; }
  inline const QStringList& search() const      { return m_search; }
  inline void setPrefix(const QString &prefix)  { m_prefix = prefix; }
  void addOther(const QString &name);
  void load(const QString &name) Q_DECL_OVERRIDE;
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
