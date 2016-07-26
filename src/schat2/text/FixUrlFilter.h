/* Simple Chat
 * Copyright (c) 2008-2016 Alexander Sedov <imp@schat.me>
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

#ifndef FIXURLFILTER_H_
#define FIXURLFILTER_H_

#include "text/TokenFilter.h"

class FixUrlFilter : public ITokenFilter
{
public:
  inline FixUrlFilter() {}
  bool filter(QList<HtmlToken> &tokens, const ChatId &id) const Q_DECL_OVERRIDE;
  int weight() const Q_DECL_OVERRIDE { return 100; }
};

#endif /* FIXURLFILTER_H_ */
