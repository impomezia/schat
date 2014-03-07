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

#include "ChatUrls.h"
#include "sglobal.h"
#include "text/FixUrlFilter.h"

bool FixUrlFilter::filter(QList<HtmlToken> &tokens, int options) const
{
  Q_UNUSED(options)

  QList<HtmlToken> out;
  QString name;
  bool remove = false;

  foreach (const HtmlToken &token, tokens) {
    if (token.type == HtmlToken::StartTag && token.tag == LS("a")) {
      HtmlATag tag(token);
      if (tag.url.startsWith(LS("chat://channel/"))) {
        ClientChannel user = ChatUrls::channel(QUrl(tag.url));
        if (user)
          name = user->name();
      }

      out.append(token);
    }
    else if (!name.isEmpty()) {
      if (name != token.text) {
        out.append(HtmlToken(name));
        out.append(HtmlToken(HtmlToken::Tag, LS("</a>")));
        remove = true;

        if (token.text.startsWith(name))
          out.append(HtmlToken(LC(' ') + token.text.mid(name.size())));
        else
          out.append(LC(' ') + token.text);
      }
      else
        out.append(token);

      name.clear();
    }
    else if (token.type == HtmlToken::EndTag && token.tag == LS("a") && remove) {
      remove = false;
    }
    else
      out.append(token);
  }

  tokens = out;
  return true;
}
