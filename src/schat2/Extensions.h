/* $Id: Extensions.h 2649 2012-05-15 13:13:32Z IMPOMEZIA $
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

#ifndef EXTENSIONS_H_
#define EXTENSIONS_H_

#include <QObject>
#include <QVariant>

#include "schat.h"

class Extension;
class ExtensionFactory;
class QThreadPool;

class SCHAT_CORE_EXPORT Extensions : public QObject
{
  Q_OBJECT

public:
  Extensions(QObject *parent = 0);
  ~Extensions();
  Extension* load(const QString &fileName, bool install = false);
  inline Extension* get(const QString &key)       { return m_extensions.value(key); }
  inline static QString root()                    { return m_root; }
  void addFactory(ExtensionFactory *factory);
  void install(const QString &key);

  static QVariantMap manifest(const QString &fileName);

signals:
  void created(const QString &key);
  void installed(const QString &key);
  void loaded();

public slots:
  void load();

private:
  void find(QStringList &extensions, const QString &path);
  void install(Extension *extension);

  QHash<QString, Extension*> m_extensions;     ///< Загруженные расширения.
  QHash<QString, ExtensionFactory*> m_factory; ///< Список объектов ExtensionFactory для создания Extension.
  QThreadPool *m_pool;                         ///< Пул потоков для запуска фоновых задач.
  static QString m_root;                       ///< Корневая директория для установки расширений.
};

#endif /* EXTENSIONS_H_ */
