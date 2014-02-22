/* $Id: NodeInit.h 3732 2013-07-07 18:55:44Z IMPOMEZIA $
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

#ifndef NODEINIT_H_
#define NODEINIT_H_

#include <QObject>
#include <QStringList>

class Core;
class NodePlugins;
class NodePool;
class Storage;

/*!
 * Загрузчик сервера.
 */
class NodeInit : public QObject
{
  Q_OBJECT

public:
  NodeInit(const QString &app = QString(), QObject *parent = 0);
  ~NodeInit();
  static bool hasVersionKey();
  static QString base();
  static void version();
  void quit();

public slots:
  void start();

private:
  Core *m_core;           ///< Указатель на объект Core.
  NodePlugins *m_plugins; ///< Загрузчик плагинов.
  NodePool *m_pool;       ///< Пул обслуживающий подключения.
  Storage *m_storage;     ///< Хранилище данных.
};

#endif /* NODEINIT_H_ */
