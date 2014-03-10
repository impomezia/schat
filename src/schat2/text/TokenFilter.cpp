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

#include "text/HtmlFilter.h"
#include "text/TokenFilter.h"

QMap<QString, QMap<int, FilterPtr> > TokenFilter::m_filters;

QString TokenFilter::filter(const QString &type, const QString &text, int options, const ChatId &id)
{
  QList<HtmlToken> tokens = HtmlFilter(options).tokenize(text);
  const QMap<int, FilterPtr> &filters = m_filters[type];

  foreach (FilterPtr filter, filters) {
    filter->filter(tokens, id);
  }

  return HtmlFilter::build(tokens);
}


void TokenFilter::add(const QString &type, ITokenFilter *filter)
{
  m_filters[type][filter->weight()] = FilterPtr(filter);
}


void TokenFilter::clear()
{
  m_filters.clear();
}
