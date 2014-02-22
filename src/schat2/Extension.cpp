/* $Id: Extension.cpp 2651 2012-05-15 17:03:01Z IMPOMEZIA $
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

#include <QFile>
#include <QDir>

#include "Extension.h"
#include "Extensions.h"
#include "JSON.h"
#include "sglobal.h"
#include "qzip/qzipreader.h"

ExtensionFactory::ExtensionFactory(const QString &type)
  : m_type(type)
{
}


Extension* ExtensionFactory::create(const QVariantMap &manifest, const QString &fileName)
{
  return new Extension(manifest, fileName);
}


ExtensionInstaller::ExtensionInstaller(Extension *extension)
  : QObject()
  , QRunnable()
  , m_file(extension->fileName())
  , m_key(extension->key())
  , m_root(extension->root())
{
}


void ExtensionInstaller::run()
{
  QDir().mkpath(m_root);
  QZipReader reader(m_file);
  if (!reader.isReadable())
    return;

  if (reader.extractAll(m_root))
    emit installed(m_key);
}


Extension::Extension(const QVariantMap &manifest, const QString &fileName)
  : m_fileName(fileName)
  , m_manifest(manifest)
{
}


QDateTime Extension::date() const
{
  return QDateTime::fromString(manifest().value(LS("date")).toString(), Qt::ISODate);
}


QString Extension::key() const
{
  return m_manifest.value(LS("type")).toString() + LC('/') + m_manifest.value(LS("id")).toString();
}


QString Extension::root() const
{
  return Extensions::root() + LC('/') + key();
}


/*!
 * Проверка на то установлено расширение или нет.
 */
bool Extension::isInstalled() const
{
  QFile file(root() + LS("/manifest.json"));
  if (!file.exists())
    return false;

  if (!file.open(QIODevice::ReadOnly))
    return false;

  QVariantMap manifest = JSON::parse(file.readAll()).toMap();
  QDateTime date = QDateTime::fromString(manifest.value(LS("date")).toString(), Qt::ISODate);
  if (!date.isValid())
    return false;

  if (date < this->date())
    return false;

  return true;
}


bool Extension::isValid() const
{
  return true;
}


ExtensionInstaller* Extension::installer()
{
  return new ExtensionInstaller(this);
}
