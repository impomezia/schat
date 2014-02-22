/* $Id: HtmlFilter.cpp 3696 2013-06-15 19:51:46Z IMPOMEZIA $
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

#include <QStringList>

#include "HtmlFilter_p.h"
#include "sglobal.h"
#include "text/HtmlFilter.h"
#include "text/PlainTextFilter.h"

static inline int h2i(char hex)
{
  if (hex >= '0' && hex <= '9')
    return hex - '0';
  if (hex >= 'a' && hex <= 'f')
    return hex - 'a' + 10;
  if (hex >= 'A' && hex <= 'F')
    return hex - 'A' + 10;
  return -1;
}


static inline int hex2int(const char *s)
{
  return (h2i(s[0]) << 4) | h2i(s[1]);
}


static inline int hex2int(char s)
{
  int h = h2i(s);
  return (h << 4) | h;
}


bool HtmlFilterPrivate::get_hex_rgb(const char *name, int *r, int *g, int *b)
{
  if (name[0] != '#')
    return false;

  name++;
  const int len = qstrlen(name);
  if (len == 12) {
    *r = hex2int(name);
    *g = hex2int(name + 4);
    *b = hex2int(name + 8);
  }
  else if (len == 9) {
    *r = hex2int(name);
    *g = hex2int(name + 3);
    *b = hex2int(name + 6);
  }
  else if (len == 6) {
    *r = hex2int(name);
    *g = hex2int(name + 2);
    *b = hex2int(name + 4);
  }
  else if (len == 3) {
    *r = hex2int(name[0]);
    *g = hex2int(name[1]);
    *b = hex2int(name[2]);
  }
  else
    *r = *g = *b = -1;

  if ((uint) *r > 255 || (uint) *g > 255 || (uint) *b > 255) {
    *r = *g = *b = -1;
    return false;
  }

  return true;
}


bool HtmlFilterPrivate::get_hex_rgb(const QChar *str, int len, int *r, int *g, int *b)
{
  if (len > 13)
    return false;

  char tmp[16];
  for (int i = 0; i < len; ++i)
    tmp[i] = str[i].toLatin1();

  tmp[len] = 0;
  return get_hex_rgb(tmp, r, g, b);
}


HtmlFilter::HtmlFilter(int options, int sizeLimit, int breaksLimit)
  : m_breaksLimit(breaksLimit)
  , m_options(options)
  , m_sizeLimit(sizeLimit)
{
}


QList<HtmlToken> HtmlFilter::tokenize(const QString &text) const
{
  QString out = prepare(text);

  QList<HtmlToken> tokens;
  tokenize(out, tokens);
  out.clear();

  if (m_optimize)
    optimize(tokens);

  for (int i = tokens.size() - 1; i >= 0; --i) {
    const HtmlToken &token = tokens.at(i);
    if ((token.type == HtmlToken::EndTag || token.type == HtmlToken::StartTag) && token.tag == LS("span")) {
      tokens.removeAt(i);
    }
  }

  if (m_sizeLimit > 0) {
    for (int i = 0; i < tokens.size(); ++i) {
      m_size += tokens.at(i).text.size();
      if (m_size > m_sizeLimit) {
        truncate(tokens, i);
        break;
      }
    }
  }

  trimmed(tokens);
  return tokens;
}


QString HtmlFilter::filter(const QString &text) const
{
  return build(tokenize(text));
}


QString HtmlFilter::build(const QList<HtmlToken> &tokens)
{
  QString out;

  for (int i = 0; i < tokens.size(); ++i) {
    out += tokens.at(i).text;
  }

  QString nbsp = out;
  nbsp.remove(LS("&nbsp;"));
  if (nbsp.simplified().isEmpty())
    return QString();

  return out;
}


bool HtmlFilter::colorValue(QString &value) const
{
  int r = -1;
  int g = -1;
  int b = -1;

  if (value.startsWith(LC('#')) && !HtmlFilterPrivate::get_hex_rgb(value.constData(), value.length(), &r, &g, &b))
    return false;

  if (value.startsWith(LS("rgb(")) ) {
    const QStringList rgb = value.mid(4, value.size() - 5).remove(LC(' ')).split(LC(','));
    if (rgb.size() == 3) {
      r = rgb.at(0).toInt();
      g = rgb.at(1).toInt();
      b = rgb.at(2).toInt();
    }
  }

  if (r == -1 && g == -1 && b == -1)
    return false;

  if (r > 210 && g > 210 && b > 210)
    return false;

  value.sprintf("#%02x%02x%02x", r, g, b);
  return true;
}


bool HtmlFilter::cssValue(const QString &property, const QString &tag, const Range &range, QString &value) const
{
  int start = -1;
  int end = range.second;

  forever {
    start = tag.lastIndexOf(property + LC(':'), end);
    if (start == -1 || start < range.first)
      return false;

    if (range.first == start)
      break;

    const QChar c = tag.at(start - 1);
    if (c == LC(' ') || c == LC(';'))
      break;

    end = start - 1;
  }

  start = start + property.size() + 1;
  end = tag.indexOf(';', start);
  if (end == -1)
    end = range.second;

  if (start == end)
    return false;

  value = tag.mid(start, end - start).simplified();
  return true;
}


bool HtmlFilter::isLastIsBreak(const QList<HtmlToken> &tokens) const
{
  if (m_breaksLimit == 0)
    return true;

  if (tokens.isEmpty())
    return false;

  if (m_breaksLimit > 0 && m_breaksLimit == m_breaks)
    return true;

  if (m_br > 1)
    return true;

  if (tokens.last().text.contains(QString("&nbsp;"))) {
    QString text = tokens.last().text;
    text.remove(QString("&nbsp;"));
    if (text.simplified().isEmpty())
      return true;
  }

  return false;
}


bool HtmlFilter::isSpace(const HtmlToken &token) const
{
  if (token.text.isEmpty())
    return true;

  if (token.text == LS(" "))
    return true;

  if (token.tag == LS("br"))
    return true;

  return false;
}


/*!
 * Поиск закрывающего тега для элемента.
 */
int HtmlFilter::endTag(const QString &tag, QList<HtmlToken> &tokens, int pos) const
{
  int gt = -1;
  for (int i = pos; i < tokens.size(); ++i) {
    if (tokens.at(i).type == HtmlToken::EndTag && tokens.at(i).tag == tag) {
      gt = i;
      break;
    }
  }

  return gt;
}


QString HtmlFilter::prepare(const QString &text) const
{
  QString out;
  if (m_options & ConvertSpacesToNbsp) {
    out = text;
    out.replace(LS("  "), LS("&nbsp; "));
    out = out.simplified();
  }
  else
    out = text.simplified();

  m_optimize = false;
  m_breaks = 0;
  m_size = 0;
  m_br = 0;

  PlainTextFilter::removeTag(out, LS("head"));
  PlainTextFilter::removeTag(out, LS("style"));
  PlainTextFilter::removeTag(out, LS("script"));

  return out;
}


/*!
 * Получение начального и конечного положения тела атрибута \p attr в входящей строке \p tag.
 */
HtmlFilter::Range HtmlFilter::attrBody(const QString &attr, const QString &tag) const
{
  Range body(-1, -1);
  body.first = tag.indexOf(attr + LC('='));
  if (body.first == -1) // атрибут не найден.
    return body;

  body.first = body.first + attr.size() + 2;
  if (tag.size() == body.first) // пустое тело атрибута.
    return Range(-1, -1);

  const QChar c = tag.at(body.first - 1);
  if (c != LC('\'') && c != LC('"')) // не допустимый символ начала тела.
    return Range(-1, -1);

  body.second = tag.indexOf(c, body.first);
  if (body.second == -1 || body.first == body.second) // конец тела не найден или пустое тело.
    return Range(-1, -1);

  return body;
}


void HtmlFilter::makeTextToken(QList<HtmlToken> &tokens, const QString &text) const
{
  HtmlToken token(text);

  if (tokens.isEmpty()) {
    tokens.append(token);
    m_br = 0;
    return;
  }

  HtmlToken &last = tokens.last();
  if (last.type == HtmlToken::Text) {
    if (text == LS(" ") && last.text == text)
      return;

    last.text.append(text);
    return;
  }

  if (last.type == HtmlToken::StartTag && text == LS(" ") && last.tag == LS("br"))
    return;

  tokens.append(token);
  m_br = 0;
}


/*!
 * Оптимизация и дополнительная фильтрация токенов.
 */
void HtmlFilter::optimize(QList<HtmlToken> &tokens) const
{
  int index = -1;
  for (int i = 0; i < tokens.size(); ++i) {
    if (!tokens.at(i).simple) {
      index = i;
      break;
    }
  }

  if (index == -1)
    return;

  HtmlToken token = tokens.at(index);

  if (token.tag == LS("a") || token.tag == LS("font")) {
    tokens[index].simple = true;
    int gt = endTag(token.tag, tokens, index);

    // Если закрывающий тег не найден, удаляем тег.
    if (gt == -1) {
      tokens.removeAt(index);
      return optimize(tokens);
    }

    // Если закрывающий следует сразу после открывающего, удаляем тег и закрывающий тег.
    if (gt - index == 1) {
      tokens.removeAt(index);
      if (index < tokens.size())
        tokens.removeAt(index);

      return optimize(tokens);
    }

    // Удаляем все не текстовые токены внутри тега.
    for (int i = gt - 1; i > index; --i) {
      if (tokens.at(i).type != HtmlToken::Text) {
        tokens.removeAt(i);
        gt--;
      }
      else {
        tokens[i].parent = token.tag;
      }
    }

    AbstractTag *tag = 0;
    if (token.tag == LS("a"))
      tag = new HtmlATag(token);
    else if (token.tag == LS("font"))
      tag = new HtmlFontTag(token);

    if (!tag->valid || gt - index == 1) {
      tokens.removeAt(gt);
      tokens.removeAt(index);
    }
    else
      tokens[index].text = tag->toText();

    delete tag;
    return optimize(tokens);
  }
  /// Тег span в зависимости от css стилей преобразуется в теги b, i, u, s и font
  /// и полностью удаляется из текста. Тег font используется для установки цвета элемента.
  else if (token.tag == LS("span")) {
    QList<HtmlToken> tags;
    const Range range = attrBody(LS("style"), token.text);

    if (range.first != -1) {
      QString value;

      if (cssValue(LS("font-weight"), token.text, range, value) && (value == LS("bold") || value == LS("bolder") || value == LS("600") || value == LS("700") || value == LS("800") || value == LS("900")))
        tags.append(HtmlToken(HtmlToken::Tag, LS("<b>")));

      if (cssValue(LS("font-style"), token.text, range, value) && value == LS("italic"))
        tags.append(HtmlToken(HtmlToken::Tag, LS("<i>")));

      if (cssValue(LS("text-decoration"), token.text, range, value)) {
        if (value == LS("underline"))
          tags.append(HtmlToken(HtmlToken::Tag, LS("<u>")));
        else if (value == LS("line-through"))
          tags.append(HtmlToken(HtmlToken::Tag, LS("<s>")));
      }

      if (cssValue(LS("color"), token.text, range, value) && colorValue(value)) {
        HtmlToken token(HtmlToken::Tag, LS("<font color=\"") + value + LS("\">"));
        token.simple = true;
        tags.append(token);
      }
    }

    tokens.removeAt(index);
    int gt = endTag(token.tag, tokens, index);

    if (gt == -1)
      return optimize(tokens);

    tokens.removeAt(gt);
    if (index == gt)
      return optimize(tokens);

    gt++;

    foreach (HtmlToken tag, tags) {
      tokens.insert(index, tag);
      tokens.insert(gt, tag.toEndTag());
      index++;
      gt++;
    }

    return optimize(tokens);
  }
}


/*!
 * Разбивает текст на токены, удаляет все не допустимые теги.
 */
void HtmlFilter::tokenize(const QString &text, QList<HtmlToken> &tokens) const
{
  QStringList simple; // Список простых тегов.
  simple.append(LS("br"));
  simple.append(LS("b"));
  simple.append(LS("i"));
  simple.append(LS("u"));
  simple.append(LS("s"));

  QStringList safe;
  if (m_options & AllowSpanTag)
    safe.append(LS("span"));

  if (m_options & AllowImgTag)
    safe.append(LS("img"));

  safe.append(LS("a"));
  safe.append(LS("font"));

  int lt = 0;
  int gt = 0;
  int pos = 0;

  for (int i = 0; i < text.size(); ++i) {
    lt = text.indexOf(LC('<'), pos);
    if (lt == -1) {
      HtmlToken token(text.mid(pos));
      tokens.append(token);
      m_br = 0;
      break;
    }

    if (lt != pos) {
      makeTextToken(tokens, text.mid(pos, lt - pos));
    }

    gt = text.indexOf(LC('>'), lt);
    pos = gt + 1;
    HtmlToken token(HtmlToken::Tag, text.mid(lt, pos - lt));

    // Закрывающий тег p преобразуется в тег br.
    if (token.type == HtmlToken::EndTag && (token.tag == LS("p") || token.tag == LS("div"))) {
      if (!tokens.isEmpty()) {
        if (token.tag == LS("div"))
          m_br++;

        if (isLastIsBreak(tokens))
          continue;

        HtmlToken token(HtmlToken::Tag, LS("<br>"));
        tokens.append(token);
        m_br++;
        m_breaks++;
      }
      continue;
    }

    // Обработка простых тегов, начальный тег принудительно приводится к простому виду.
    if (simple.contains(token.tag)) {
      if (token.type == HtmlToken::StartTag && !token.simple) {
        token.simple = true;
        token.text = LC('<') + token.tag + LC('>');
      }

      if (token.tag == LS("br")) {
        if (isLastIsBreak(tokens))
          continue;

        m_br++;
        m_breaks++;
      }

      tokens.append(token);
    }

    if (safe.contains(token.tag)) {
      if ((m_options & AllowImgTag) && token.tag == LS("img")) {
        HtmlImgTag tag(token);
        if (tag.valid)
          tokens.append(HtmlToken(tag.toText()));
      }
      else {
        m_optimize = true;
        tokens.append(token);
        m_br = 0;
      }
    }
  }

  trimmed(tokens);
}



void HtmlFilter::trimmed(QList<HtmlToken> &tokens) const
{
  forever {
    if (!tokens.isEmpty() && isSpace(tokens.first()))
      tokens.removeFirst();
    else
      break;
  }

  forever {
    if (!tokens.isEmpty() && isSpace(tokens.last()))
      tokens.removeLast();
    else
      break;
  }
}


/*!
 * Ограничение максимальной длины текста.
 * В зависимости от html разметки результирующий размер может быть немного меньше или больше лимита.
 *
 * \param tokens Токены.
 * \param pos    Индекс токена на котором было обнаружено превышение размера.
 */
void HtmlFilter::truncate(QList<HtmlToken> &tokens, int pos) const
{
  HtmlToken &token = tokens[pos];

  if (token.type == HtmlToken::Text) {
    token.text = token.text.left(token.text.size() - (m_size - m_sizeLimit));
    if (token.text.isEmpty())
       return truncate(tokens, --pos);

    ++pos;

    for (int i = pos; i < tokens.size(); ++i) {
      if (token.type != HtmlToken::EndTag) {
        pos = i;
        break;
      }
    }
  }
  else if (token.type == HtmlToken::StartTag) {
    for (int i = pos; i >= 0; --i) {
      if (token.type != HtmlToken::StartTag) {
        pos = ++i;
        break;
      }
    }
  }
  else if (token.type == HtmlToken::EndTag) {
    ++pos;

    for (int i = pos; i < tokens.size(); ++i) {
      if (token.type != HtmlToken::EndTag) {
        pos = i;
        break;
      }
    }
  }

  for (int i = tokens.size(); i >= pos; --i) {
    tokens.removeAt(i);
  }
}
