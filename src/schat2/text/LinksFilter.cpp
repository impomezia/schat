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

#include "text/LinksFilter.h"
#include "sglobal.h"

LinksFilter::LinksFilter()
{
  m_scheme += LS("http://");
  m_scheme += LS("https://");
  m_scheme += LS("ftp://");
  m_scheme += LS("mailto:");
}


bool LinksFilter::filter(QList<HtmlToken> &tokens, const ChatId &id) const
{
  Q_UNUSED(id)

  QList<HtmlToken> out;

  for (int i = 0; i < tokens.size(); ++i) {
    HtmlToken token = tokens.at(i);
    if (token.type == HtmlToken::Text && token.parent != LS("a"))
      parse(out, token.text);
    else
      out.append(token);
  }

  tokens = out;
  return false;
}


QString LinksFilter::url(const QString &text, int index, int &last) const
{
  last = text.indexOf(LC(' '), index);
  QString url;
  last == -1 ? url = text.mid(index) : url = text.mid(index, last - index);

  if (url.endsWith(LS("&nbsp;")))
    return url.mid(0, url.size() - 6);

  return url;
}


void LinksFilter::makeUrl(QList<HtmlToken> &tokens, const QString &url, const QString &text) const
{
  HtmlToken a(HtmlToken::Tag, HtmlATag(url, url).toText());
  tokens.append(a);

  HtmlToken tag(text);
  tag.parent = LS("a");
  tokens.append(tag);

  tokens.append(a.toEndTag());
}


/*!
 * Поиск ссылок в тексте и автоматическое преобразование их в html ссылки.
 */
void LinksFilter::parse(QList<HtmlToken> &tokens, const QString &text) const
{
  int index = -1;
  int last = -1;
  QString url;

  /// - http/https/ftp полный список в \p m_scheme.
  for (int i = 0; i < m_scheme.size(); ++i) {
    index = text.indexOf(m_scheme.at(i));
    if (index != -1) {
      if (index > 0)
        tokens.append(HtmlToken(text.left(index)));

      url = this->url(text, index, last);
      makeUrl(tokens, url, url);

      if (last != -1)
        return parse(tokens, text.mid(last));

      return;
    }
  }


  /// - Ссылки вида www.exampe.com в преобразуются в http.
  index = text.indexOf(LS("www."));
  if (index != -1) {
    url = this->url(text, index, last);

    if (url.count(LC('.')) > 1) {
      if (index > 0)
        tokens.append(HtmlToken(text.left(index)));

      makeUrl(tokens, LS("http://") + url, url);

      if (last != -1)
        return parse(tokens, text.mid(last));

      return;
    }

    if (last != -1) {
      tokens.append(HtmlToken(text.left(last)));
      return parse(tokens, text.mid(last));
    }
  }

  /// - Ссылки вида user@example.com преобразуются в mailto.
  index = text.indexOf(LC('@'));
  if (index != -1) {
    int start = text.lastIndexOf(LC(' '), index);
    QString name = text.mid(start + 1, index - start - 1);
    last = -1;

    if (!name.isEmpty()) {
      url = this->url(text, index, last);
      if (url.contains(LC('.'))) {
        if (index > 0) {
          tokens.append(HtmlToken(text.left(index - name.size())));

          makeUrl(tokens, LS("mailto:") + name + url, name + url);

          if (last != -1)
            return parse(tokens, text.mid(last));

          return;
        }
      }
    }

    if (last != -1) {
      tokens.append(HtmlToken(text.left(last)));
      return parse(tokens, text.mid(last));
    }
  }

  tokens.append(HtmlToken(text));
}
