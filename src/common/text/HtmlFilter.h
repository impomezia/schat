/* $Id: HtmlFilter.h 3696 2013-06-15 19:51:46Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#ifndef HTMLFILTER_H_
#define HTMLFILTER_H_

#include <QVariant>

#include "schat.h"
#include "text/HtmlToken.h"

class QStringList;

/*!
 * Фильтрует и вырезает всё лишнее из HTML оставляя только минимальное
 * безопасное содержимое.
 */
class SCHAT_EXPORT HtmlFilter
{
public:
  /// Опции фильтрации.
  enum Options {
    NoOptions = 0,           ///< Нет специальных опций.
    ConvertSpacesToNbsp = 1, ///< Конвертировать двойные пробелы в неразрывный пробел.
    AllowSpanTag = 2,        ///< Разрешить поддержку тега span.
    AllowImgTag = 4          ///< Добавляет поддержку img тега, он будет преобразован в текст равный атрибуту alt.
  };

  typedef QPair<int, int> Range;

  HtmlFilter(int options = NoOptions, int sizeLimit = 8000, int breaksLimit = 30);
  QList<HtmlToken> tokenize(const QString &text) const;
  QString filter(const QString &text) const;
  static QString build(const QList<HtmlToken> &tokens);

private:
  bool colorValue(QString &value) const;
  bool cssValue(const QString &property, const QString &tag, const Range &range, QString &value) const;
  bool isLastIsBreak(const QList<HtmlToken> &tokens) const;
  bool isSpace(const HtmlToken &token) const;
  int endTag(const QString &tag, QList<HtmlToken> &tokens, int pos = 0) const;
  QString prepare(const QString &text) const;
  Range attrBody(const QString &attr, const QString &tag) const;
  void makeTextToken(QList<HtmlToken> &tokens, const QString &text) const;
  void optimize(QList<HtmlToken> &tokens) const;
  void tokenize(const QString &text, QList<HtmlToken> &tokens) const;
  void trimmed(QList<HtmlToken> &tokens) const;
  void truncate(QList<HtmlToken> &tokens, int pos) const;

  int m_breaksLimit;       ///< Ограничение на число переносов строк.
  int m_options;           ///< Опции фильтрации.
  int m_sizeLimit;         ///< Мягкое ограничение на размер, может быть незначительно превышено для того чтобы не портить разметку.
  mutable bool m_optimize; ///< true если требуется оптимизация тегов.
  mutable int m_br;
  mutable int m_breaks;    ///< Текущее число переносов строк.
  mutable int m_size;      ///< Текущий результирующий объём текста.
};

#endif /* HTMLFILTER_H_ */
