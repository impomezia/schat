/* $Id: AuthIcon.h 2942 2012-07-29 17:59:59Z IMPOMEZIA $
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

#ifndef AUTHICON_H_
#define AUTHICON_H_

#include <QIcon>

#include "schat.h"

class SCHAT_CORE_EXPORT AuthIcon
{
public:
  AuthIcon();
  static QIcon icon(const QString &provider);
  static QPixmap pixmap(const QString &provider);

private:
  QPixmap m_layout;
  static AuthIcon *m_self;
};

#endif /* AUTHICON_H_ */
