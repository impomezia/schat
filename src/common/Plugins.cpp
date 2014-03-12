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

#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>

#include "Plugins.h"
#include "sglobal.h"
#include "tools/Ver.h"

/*!
 * Загружает плагин с именем \p fileName.
 *
 * В случае успешной загрузки метод isValid() вернёт true.
 */
PluginItem::PluginItem(const QString &fileName)
  : m_loaded(false)
  , m_valid(false)
  , m_plugin(0)
{
  m_loader.setFileName(fileName);
  if (!m_loader.load())
    return;

  m_plugin = m_loader.instance();
  if (!m_plugin)
    return;

  CoreApi *api = qobject_cast<CoreApi *>(m_plugin);
  if (!api)
    return;

  m_header = api->header();
  if (m_header.value(CORE_API_ID).toString().isEmpty())
    return;

  if (m_header.value(CORE_API_NAME).toString().isEmpty())
    return;

  m_valid = true;
}


PluginItem::~PluginItem()
{
  if (m_loader.isLoaded())
    m_loader.unload();
}


QString PluginItem::icon() const
{
  QString file = LS(":/images/") + id() + LS("/icon.png");
  if (QFile::exists(file))
    return file;

  return LS(":/images/plugin32.png");
}


Plugins::Plugins(QObject *parent)
  : QObject(parent)
{
}


Plugins::~Plugins()
{
  for (unsigned i = m_list.size(); i-- > 0;) {
    delete m_list[i];
  }
}


inline bool priorityLessThan(PluginItem *p1, PluginItem *p2)
{
  if (p1->priority() == p2->priority())
    return p1->id().toLower() < p2->id().toLower();

  return p1->priority() > p2->priority();
}


/*!
 * Загрузка плагинов.
 */
void Plugins::load()
{
  QStringList paths = QCoreApplication::libraryPaths();
  paths.removeAll(QCoreApplication::applicationDirPath());

  foreach (const QString &path, paths) {
    load(path);
  }

  qSort(m_list.begin(), m_list.end(), priorityLessThan);

  init();
}


/*!
 * Выполняет проверку интерфейса плагина.
 *
 * \return true если плагин прошёл проверку.
 */
bool Plugins::check(PluginItem *plugin)
{
  if (m_plugins.contains(plugin->id()))
    return false;

  if (m_type != plugin->header().value(CORE_API_TYPE).toString())
    return false;

  // Отключаем плагин если требуемая версия не совместима.
  if (Ver::current() != Ver(plugin->header().value(CORE_API_REQUIRED).toString()))
    return false;

  return true;
}


/*!
 * Загрузка плагинов с интерфейсом CoreApi из папки \p path
 * в список плагинов \p m_plugins.
 */
void Plugins::load(const QString &path)
{
  QDir dir(path);
  QStringList filters;

# if defined(Q_OS_WIN32) || defined(Q_OS_SYMBIAN)
  filters.append(LS("*.dll"));
# elif defined(Q_OS_MAC)
  filters.append(LS("*.dylib"));
  filters.append(LS("*.so"));
# else
  filters.append(LS("*.so"));
# endif

  QStringList files = dir.entryList(filters, QDir::Files);

  for (int i = 0; i < files.size(); ++i) {
    PluginItem *item = new PluginItem(dir.absoluteFilePath(files.at(i)));
    if (!item->isValid()) {
      delete item;
      continue;
    }

    if (!check(item)) {
      delete item;
      continue;
    }

    m_plugins.insert(item->id(), item);
    m_list.append(item);
  }
}
