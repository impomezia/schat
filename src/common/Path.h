/* $Id: Path.h 3516 2013-02-23 15:37:09Z IMPOMEZIA $
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

#ifndef PATH_H_
#define PATH_H_

#include <QStringList>

#include "schat.h"

#if defined(Q_OS_LINUX) && !defined(SCHAT_PREFIX)
# define SCHAT_PREFIX "/usr"
#endif

/*!
 * Предоставляет унифицированный доступ к файловым ресурсам чата.
 */
class SCHAT_EXPORT Path
{
  Path() {}

public:
  enum Scope {
    UserScope,
    SystemScope
  };

  static void init(const QString &app = QString());
  static void initWithBase(const QString &base);

  inline static bool isPortable()    { return m_portable; }
  static QString app();
  static QString appDirPath();
  static QString cache();
  static QString config();
  static QString data(Scope scope = UserScope);
  static QString file(const QString &path, const QString &name);
  static QStringList plugins();

private:
  static bool m_portable;      ///< \b true если все файлы чат хранит в своей папке.
  static QString m_app;        ///< Имя чата полученное на основе имени исполняемого файла.
  static QString m_appDirPath; ///< Папка чата.
};

#endif /* PATH_H_ */
