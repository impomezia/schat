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
#include <QImageReader>

#include "PreviewCore.h"
#include "PreviewFilter.h"
#include "sglobal.h"

PreviewFilter::PreviewFilter(PreviewCore *core)
  : m_core(core)
{
  const QList<QByteArray> formats = QImageReader::supportedImageFormats();

  foreach (const QByteArray &format, formats) {
    m_formats.append(LS(".") + format);
  }
}


bool PreviewFilter::filter(QList<HtmlToken> &tokens, const ChatId &id) const
{
  QList<QUrl> urls;

  foreach (const HtmlToken &token, tokens) {
    if (token.type != HtmlToken::StartTag || token.tag != LS("a"))
      continue;

    const QUrl url(HtmlATag(token).url);
    if (isProbablyImage(url))
      urls.append(url);
  }

  if (!urls.isEmpty())
    m_core->add(id, urls);

  return true;
}


bool PreviewFilter::isProbablyImage(const QUrl &url) const
{
  if (url.scheme() != LS("http") && url.scheme() != LS("https"))
    return false;

  const QString path = url.path().toLower();

  foreach (const QString &format, m_formats) {
    if (path.endsWith(format))
      return true;
  }

  return false;
}
