/* $Id: PlainTextFilter.cpp 3320 2012-12-14 21:11:25Z IMPOMEZIA $
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

#include "sglobal.h"
#include "text/PlainTextFilter.h"

QString PlainTextFilter::filter(const QString &text)
{
  QString out = text;

  out.replace(LC('\n'), QString());
  out.replace(LS("</p>"), LS("\n"), Qt::CaseInsensitive);
  out.replace(LS("<br />"), LS("\n"), Qt::CaseInsensitive);

  removeTag(out, LS("style"));
  removeTag(out, LS("script"));

  int lt = 0;
  int gt = 0;
  forever {
    lt = out.indexOf(LC('<'), lt);
    if (lt == -1)
      break;

    gt = out.indexOf(LC('>'), lt);
    if (gt == -1) {
      out.remove(lt, out.size() - lt);
      break;
    }

    out.remove(lt, gt - lt + 1);
  }

  out.replace(LS("&gt;"),         LS(">"));
  out.replace(LS("&lt;"),         LS("<"));
  out.replace(LS("&quot;"),       LS("\""));
  out.replace(LS("&nbsp;"),       LS(" "));
  out.replace(LS("&amp;"),        LS("&"));
  out.replace(QChar(QChar::Nbsp), LS(" "));
  out = out.trimmed();
  return out;
}


void PlainTextFilter::removeTag(QString &text, const QString &tag)
{
  int lt = 0;
  int gt = 0;
  forever {
    lt = text.indexOf(QLatin1String("<") + tag, lt, Qt::CaseInsensitive);
    if (lt == -1)
      break;

    gt = text.indexOf(QLatin1String("</") + tag + QLatin1String(">"), lt, Qt::CaseInsensitive);
    if (gt == -1) {
      ++lt;
      continue;
    }

    text.remove(lt, gt - lt + tag.size() + 3);
  }
}
