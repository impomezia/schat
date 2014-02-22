/* $Id: QtEscape.h 3490 2013-02-11 02:15:27Z IMPOMEZIA $
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

#ifndef QTESCAPE_H_
#define QTESCAPE_H_

#if QT_VERSION >= 0x050000
# include <QString>

namespace Qt {

  inline QString escape(const QString &plain) {
    return plain.toHtmlEscaped();
  }

}
#else
# include <QTextDocument>
#endif

#endif /* QTESCAPE_H_ */
