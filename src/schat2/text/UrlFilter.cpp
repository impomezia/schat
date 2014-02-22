/* $Id: UrlFilter.cpp 3650 2013-04-21 00:21:16Z IMPOMEZIA $
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

#include <QUrl>

#include "ChatUrls.h"
#include "net/SimpleID.h"
#include "QtEscape.h"
#include "sglobal.h"
#include "text/UrlFilter.h"

UrlFilter::UrlFilter()
  : AbstractFilter(100)
{
}


bool UrlFilter::filter(QList<HtmlToken> &tokens, int options) const
{
  Q_UNUSED(options)

  QString name;

  for (int i = 0; i < tokens.size(); ++i) {
    const HtmlToken &token = tokens.at(i);
    if (token.type == HtmlToken::StartTag && token.tag == LS("a")) {
      HtmlATag tag(tokens.at(i));

      if (tag.url.startsWith(LS("chat://channel/"))) {
        tag.classes = LS("nick");
        ClientChannel user = ChatUrls::channel(QUrl(tag.url));
        if (user) {
          tag.classes += LC(' ') + SimpleID::encode(user->id());
          name = user->name();

          tag.classes += LS(" color-") + Gender::colorToString(user->gender().color());
        }

        tokens[i].text = tag.toText();
      }
      else if (tag.title.isEmpty()) {
        tag.title = tag.url;
        tokens[i].text = tag.toText();
      }
    }
    else if (token.type == HtmlToken::Text && !name.isEmpty()) {
      tokens[i].text = Qt::escape(name);
      name.clear();
    }
  }

  return true;
}
