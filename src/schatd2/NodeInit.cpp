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

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QTimer>

#include "Client.h"
#include "cores/Core.h"
#include "debugstream.h"
#include "feeds/FeedsCore.h"
#include "Heartbeat.h"
#include "net/NodePool.h"
#include "NodeInit.h"
#include "NodePlugins.h"
#include "Path.h"
#include "Settings.h"
#include "sglobal.h"
#include "Storage.h"
#include "tools/CmdLine.h"
#include "version.h"

/*!
 * Инициализация сервера.
 */
NodeInit::NodeInit(const QString &app, QObject *parent)
  : QObject(parent)
  , m_core(0)
  , m_pool(0)
{
  new FeedsCore(this);

  m_storage = new Storage(0, app, this);
  m_core    = new Core(this);
  m_plugins = new NodePlugins(this);

  CmdLine::createPid(Path::app());

  m_client = new Client(this);
  m_client->addListener(new Heartbeat(this));

  m_client->open(m_storage->settings()->value(STORAGE_API_HOST).toString(), m_storage->settings()->value(STORAGE_API_PORT).toUInt());

  QTimer::singleShot(0, this, SLOT(start()));
}


NodeInit::~NodeInit()
{
  CmdLine::removePid(Path::app());
}


bool NodeInit::hasVersionKey()
{
  return CmdLine::has(QStringList() << LS("--version") << LS("-version"));
}


QString NodeInit::base()
{
  return CmdLine::value(QStringList() << LS("--base") << LS("-base") << LS("-B")).toString();
}


void NodeInit::version()
{
  QTextStream out(stdout);
  out << LS("Simple Chat Daemon, version ") << SCHAT_VERSION;
  if (SCHAT_VER_PATH)
    out << LC('.') << SCHAT_VER_PATH;

  out << endl << endl;
  out << SCHAT_COPYRIGHT << LS(", https://schat.me") << endl;
}


void NodeInit::quit()
{
  if (m_pool) {
    m_pool->quit();
    m_pool->wait();
    delete m_pool;
  }

  if (m_core)
    m_core->quit();
}


void NodeInit::start()
{
  if (m_storage->start())
    return;

  m_plugins->load();

  const QStringList listen = Storage::settings()->value(STORAGE_LISTEN).toStringList();
  const int workers = Storage::settings()->value(STORAGE_WORKERS).toInt();
  m_pool = new NodePool(listen, workers, m_core);
  connect(m_pool, SIGNAL(ready(QObject*)), m_core, SLOT(workerReady(QObject*)));

  m_core->start();
  m_pool->start();

  m_storage->load();
}
