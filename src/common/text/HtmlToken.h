/* $Id: HtmlToken.h 2695 2012-05-23 09:12:45Z IMPOMEZIA $
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

#ifndef HTMLTOKEN_H_
#define HTMLTOKEN_H_

#include <QString>

class HtmlToken
{
public:
  enum Type {
    Undefined,
    StartTag,
    EndTag,
    Text,
    Tag
  };

  inline HtmlToken()
  : simple(true)
  , type(Undefined)
  {}

  inline HtmlToken(const QString &text)
  : simple(true)
  , text(text)
  , type(Text)
  {}

  HtmlToken(Type type, const QString &text)
  : simple(true)
  , text(text)
  , type(type)
  {
    if (type != Tag)
      return;

    tag = text.mid(1, text.size() - 2);
    if (tag.startsWith(QLatin1Char('/'))) {
      this->type = EndTag;
      tag.remove(0, 1);
    }
    else {
      this->type = StartTag;
      int space = tag.indexOf(QLatin1Char(' '));
      if (space != -1) {
        simple = false;
        tag.remove(space, tag.size() - space);
      }
    }

    if (tag.isEmpty())
      this->type = Undefined;

    tag = tag.toLower();
  }

  HtmlToken toEndTag() const
  {
    if (type != StartTag)
      return QString();

    return HtmlToken(HtmlToken::EndTag, QLatin1String("</") + tag + QLatin1Char('>'));
  }

  bool simple;    ///< false в случае если начальный тег содержит дополнительные данные.
  QString parent; ///< Родительский тег.
  QString tag;    ///< Тег, в нижнем регистре и без обрамления.
  QString text;   ///< Текстовое содержимое.
  Type type;      ///< Тип.
};


class AbstractTag
{
public:
  AbstractTag()
  : valid(false)
  {}

  virtual ~AbstractTag() {}
  virtual QString toText() const = 0;

  QString attr(const QString &tag, const QString &attr)
  {
    int size = attr.size();
    int index = tag.indexOf(attr);
    QString q = tag.mid(index + size, 1);
    int last = tag.indexOf(q, index + size + 1);
    if (last == -1)
      return QString();

    return tag.mid(index + size + 1, last - index - size - 1);
  }

  bool valid;
  QString classes;
};


class HtmlATag : public AbstractTag
{
public:
  HtmlATag(const QString &url, const QString &title = QString())
  : AbstractTag()
  , title(title)
  , url(url)
  {}

  HtmlATag(const HtmlToken &token)
  : AbstractTag()
  {
    if (token.type != HtmlToken::StartTag)
      return;

    if (token.tag != QLatin1String("a"))
      return;

    url = attr(token.text, QLatin1String("href="));
    if (!url.isEmpty())
      valid = true;
  }

  QString toText() const
  {
    QString out = QLatin1String("<a href=\"") + url + QLatin1Char('"');
    if (!classes.isEmpty())
      out += QLatin1String(" class=\"") + classes + QLatin1Char('"');

    if (!title.isEmpty())
      out += QLatin1String(" title=\"") + title + QLatin1Char('"');

    out += QLatin1String(">");
    return out;
  }

  QString title;
  QString url;
};


class HtmlFontTag : public AbstractTag
{
public:
  HtmlFontTag(const HtmlToken &token)
  : AbstractTag()
  {
    if (token.type != HtmlToken::StartTag)
      return;

    if (token.tag != QLatin1String("font"))
      return;

    color = attr(token.text, QLatin1String("color="));
    if (!color.isEmpty())
      valid = true;
  }

  QString toText() const
  {
    return QLatin1String("<font color=\"") + color + QLatin1String("\">");
  }

  QString color;
};


class HtmlImgTag : public AbstractTag
{
public:
  HtmlImgTag(const HtmlToken &token)
  : AbstractTag()
  {
    if (token.type != HtmlToken::StartTag)
      return;

    if (token.tag != QLatin1String("img"))
      return;

    alt = attr(token.text, QLatin1String("alt="));
    if (!alt.isEmpty())
      valid = true;
  }

  QString toText() const
  {
    return QChar(QChar::Nbsp) + alt;
  }

  QString alt;
};

#endif /* HTMLTOKEN_H_ */
