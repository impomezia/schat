/* $Id: Extensions.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QDebug>

#include <QThreadPool>
#include <QTimer>
#include <QDir>

#include "Extension.h"
#include "Extensions.h"
#include "JSON.h"
#include "Path.h"
#include "qzip/qzipreader.h"
#include "sglobal.h"

QString Extensions::m_root;

Extensions::Extensions(QObject *parent)
  : QObject(parent)
{
  m_pool = new QThreadPool(this);
  m_pool->setMaxThreadCount(1);
  m_root = Path::cache();

  QTimer::singleShot(0, this, SLOT(load()));
}


Extensions::~Extensions()
{
  qDeleteAll(m_factory);
  qDeleteAll(m_extensions);
}


/*!
 * Загрузка расширения.
 *
 * \param fileName Имя файла расширения.
 * \param install  \b true Если необходима автоматическая установка.
 *
 * \return 0 в случае ошибки, или созданный объект Extension.
 */
Extension* Extensions::load(const QString &fileName, bool install)
{
  QVariantMap manifest = Extensions::manifest(fileName);
  if (manifest.isEmpty())
    return 0;

  QString id = manifest.value(LS("id")).toString();
  if (id.isEmpty() || id.contains(LC('/')))
    return 0;

  QString type = manifest.value(LS("type")).toString();
  if (type.isEmpty() || type.contains(LC('/')))
    return 0;

  if (!m_factory.contains(type))
    return 0;

  Extension *extension = m_factory.value(type)->create(manifest, fileName);
  if (!extension->isValid()) {
    delete extension;
    return 0;
  }

  QString key = extension->key();
  if (m_extensions.contains(key)) {
    Extension *e = m_extensions.value(key);
    delete e;
  }

  m_extensions[key] = extension;
  emit created(key);

  if (install)
    this->install(extension);

  return extension;
}


void Extensions::addFactory(ExtensionFactory *factory)
{
  if (m_factory.contains(factory->type())) {
    ExtensionFactory *f = m_factory.value(factory->type());
    delete f;
  }

  m_factory[factory->type()] = factory;
}


void Extensions::install(const QString &key)
{
  if (!m_extensions.contains(key))
    return;

  install(m_extensions.value(key));
}


/*!
 * Низкоуровневая функция для загрузки манифеста расширения.
 */
QVariantMap Extensions::manifest(const QString &fileName)
{
  if (!QFile::exists(fileName))
    return QVariantMap();

  QZipReader reader(fileName);
  if (!reader.isReadable())
    return QVariantMap();

  return JSON::parse(reader.fileData(LS("manifest.json"))).toMap();
}


/*!
 * Загрузка всех доступных расширений.
 */
void Extensions::load()
{
  QString dir = Path::data(Path::SystemScope) + LS("/extensions");
  QStringList files;
  find(files, dir);

  foreach (const QString &file, files) {
    load(file);
  }

  emit loaded();
}


/*!
 * Рекурсивный поиск файлов расширений.
 */
void Extensions::find(QStringList &extensions, const QString &path)
{
  QDir dir(path);

  QFileInfoList files = dir.entryInfoList(QStringList(LS("*.schat")), QDir::Files);
  foreach (const QFileInfo &info, files) {
    extensions.append(info.absoluteFilePath());
  }

  files = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
  foreach (const QFileInfo &info, files) {
    find(extensions, info.absoluteFilePath());
  }
}


void Extensions::install(Extension *extension)
{
  if (!extension)
    return;

  if (extension->isInstalled()) {
    emit installed(extension->key());
    return;
  }

  ExtensionInstaller *installer = extension->installer();
  connect(installer, SIGNAL(installed(QString)), SIGNAL(installed(QString)));
  m_pool->start(installer);
}
