/* $Id: ClientCmd.h 1924 2011-12-03 19:58:47Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2011 IMPOMEZIA <schat@impomezia.com>
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

#ifndef CLIENTCMD_H_
#define CLIENTCMD_H_

#include <QString>

#include "schat.h"

class SCHAT_EXPORT ClientCmd
{
public:
  ClientCmd(const QString &text);
  inline bool isBody() const            { return m_isBody; }
  inline bool isValid() const           { return m_isValid; }
  inline const QString& body() const    { return m_body; }
  inline const QString& command() const { return m_command; }

private:
  bool m_isBody;     ///< true если \p m_body не пустая строка.
  bool m_isValid;    ///< true если команда валидна.
  QString m_body;    ///< Тело команды.
  QString m_command; ///< Команда приведённая к нижнему регистру.
};

#endif /* CLIENTCMD_H_ */
