/* $Id: Extension.h 2716 2012-05-28 18:50:44Z IMPOMEZIA $
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

#ifndef EXTENSION_H_
#define EXTENSION_H_

#include <QVariant>
#include <QDateTime>
#include <QRunnable>

#include "schat.h"

class Extension;

/*!
 * Базовый класс фабрики для создания обработчиков расширений нужных типов.
 */
class SCHAT_CORE_EXPORT ExtensionFactory
{
public:
  ExtensionFactory(const QString &type);
  virtual ~ExtensionFactory() {}

  inline const QString& type() const { return m_type; }
  virtual Extension* create(const QVariantMap &manifest, const QString &fileName);

private:
  QString m_type;
};


/*!
 * Установщик расширения.
 */
class SCHAT_CORE_EXPORT ExtensionInstaller : public QObject, public QRunnable
{
  Q_OBJECT

public:
  ExtensionInstaller(Extension *extension);
  void run();

signals:
  void installed(const QString &key);

private:
  const QString m_file; ///< Имя файла расширения.
  const QString m_key;  ///< Ключ расширения.
  const QString m_root; ///< Папка установки.
};


/*!
 * Базовый класс для расширений чата.
 */
class SCHAT_CORE_EXPORT Extension
{
public:
  Extension(const QVariantMap &manifest, const QString &fileName);
  virtual ~Extension() {}
  inline const QString& fileName() const     { return m_fileName; }
  inline const QVariantMap& manifest() const { return m_manifest; }
  inline QString id() const                  { return m_manifest.value(QLatin1String("id")).toString(); }
  inline QString name() const                { return m_manifest.value(QLatin1String("name")).toString(); }
  inline QString type() const                { return m_manifest.value(QLatin1String("type")).toString(); }
  QDateTime date() const;
  QString key() const;
  QString root() const;
  virtual bool isInstalled() const;
  virtual bool isValid() const;
  virtual ExtensionInstaller* installer();

private:
  QString m_fileName;     ///< Имя файла расширения.
  QVariantMap m_manifest; ///< Данные манифеста расширирения.
};

#endif /* EXTENSION_H_ */
