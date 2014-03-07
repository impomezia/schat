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

#include <QUrl>

#include "net/SimpleID.h"
#include "sglobal.h"
#include "YouTubeFilter.h"
#include "UrlQuery.h"

YouTubeFilter::YouTubeFilter()
{
  m_hosts << LS("www.youtube.com") << LS("youtu.be");
}


bool YouTubeFilter::filter(QList<HtmlToken> &tokens, int options) const
{
  Q_UNUSED(options)

  for (int i = 0; i < tokens.size(); ++i) {
    if (tokens.at(i).type == HtmlToken::StartTag && tokens.at(i).tag == LS("a")) {
      HtmlATag tag(tokens.at(i));

      const QString u = tag.url.replace(LS("&amp;"), LS("&"));
      const QUrl url(u);
      if (!m_hosts.contains(url.host()))
        continue;

      const QString vid = QUrlQuery(url).queryItemValue(LS("v"));
      if (vid.size() != 11)
        continue;

      tag.classes += LS("youtube");
      tokens[i].text = QString(LS("<a href=\"%1\" class=\"%2\" title=\"%1\" data-youtube-v=\"%3\">"))
          .arg(tag.url)
          .arg(tag.classes)
          .arg(vid);
    }
  }

  return true;
}
