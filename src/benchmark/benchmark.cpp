/* $Id: benchmark.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QCoreApplication>
#include <QSettings>
#include <QTimer>
#include <QFile>

#include "benchmark.h"
#include "BenchmarkClient.h"
#include "client/SimpleClient.h"
#include "JSON.h"
#include "net/SimpleID.h"

/*!
 * Конструктор класса Benchmark.
 */
Benchmark::Benchmark(QObject *parent)
  : QObject(parent)
  , m_accepted(0)
  , m_connectInterval(500)
  , m_count(0)
  , m_disconnected(0)
  , m_rejected(0)
  , m_synced(0)
  , m_usersCount(2)
  , m_nickPrefix("test_")
{
}


Benchmark::~Benchmark()
{
}


void Benchmark::quit()
{
}


void Benchmark::start()
{
  QSettings settings(QCoreApplication::applicationDirPath() + "/benchmark.conf", QSettings::IniFormat, this);
  settings.setIniCodec("UTF-8");
  m_connectInterval = settings.value("ConnectInterval", m_connectInterval).toInt();
  m_usersCount      = settings.value("UsersCount", m_usersCount).toInt();
  m_nickPrefix      = settings.value("NickPrefix", m_nickPrefix).toString();
  m_serverAddr      = settings.value("Url", m_serverAddr).toString();

  QFile file(QCoreApplication::applicationDirPath() + "/benchmark.cache");
  if (file.open(QFile::ReadOnly)) {
    QVariantMap cache = JSON::parse(file.readAll()).toMap();
    m_authIn = cache.value("auth").toList();
  }

  connectToHost();
}


void Benchmark::clientStateChanged(int state, int previousState)
{
  if (state == SimpleClient::ClientOnline) {
    ++m_accepted;
    emit accepted(m_accepted);

    SimpleClient *client = qobject_cast<SimpleClient*>(sender());
    if (!client)
      return;

    QVariantMap data;
    data["nick"]     = client->nick();
    data["uniqueId"] = SimpleID::encode(client->uniqueId());
    data["cookie"]   = SimpleID::encode(client->cookie());
    m_authOut.append(data);

    qDebug() << client->nick();
  }
  else if (previousState == SimpleClient::ClientOnline) {
    ++m_disconnected;
    emit disconnected(m_disconnected);
  }
  qDebug() << sender();
}


void Benchmark::connectToHost()
{

  if (m_count < m_usersCount) {
    BenchmarkClient *client;
    if (m_authIn.size() > m_count)
      client = new BenchmarkClient(m_authIn.at(m_count).toMap(), this);
    else
      client = new BenchmarkClient(m_nickPrefix + QString::number(m_count), this);

    connect(client->io(), SIGNAL(clientStateChanged(int,int)), SLOT(clientStateChanged(int,int)));
    client->open(m_serverAddr);

//    client->setNick(m_nickPrefix + QString::number(m_count));
//    client->setCookieAuth(false);
//    client->openUrl(m_serverAddr);

//    QSettings settings(m_chatConf, QSettings::IniFormat, this);
//    settings.setIniCodec("UTF-8");
//    settings.setValue("Profile/Nick", m_nickPrefix + QString::number(m_count));
//
//    QProcess *chat = new QProcess(this);
//    m_process.append(chat);
//
//    chat->start(m_chatFile);
//    chat->waitForStarted();
//    AbstractProfile *profile = new AbstractProfile();
//    profile->setNick(m_nickPrefix + QString::number(m_count));
//    connect(this, SIGNAL(finished()), profile, SLOT(deleteLater()));

//    ClientService *service = new ClientService(profile, m_network);
//    connect(this, SIGNAL(finished()), service, SLOT(deleteLater()));

    #if !defined(BENCHMARK_NO_UI)
//    connect(service, SIGNAL(accessGranted(QString,QString,quint16)), SLOT(accessGranted(QString,QString,quint16)));
//    connect(service, SIGNAL(accessDenied(quint16)), SLOT(accessDenied(quint16)));
//    connect(service, SIGNAL(syncUsersEnd()), SLOT(syncUsersEnd()));
//    connect(service, SIGNAL(unconnected(bool)), SLOT(unconnected()));
    emit started(++m_count);
    #else
    ++m_count;
    #endif

//    msleep(m_connectInterval);
//    connectToHost();

//    service->connectToHost();
    QTimer::singleShot(m_connectInterval, this, SLOT(connectToHost()));
  }
  else {
    QVariantMap cache;
    cache["auth"] = m_authOut;

    QFile file(QCoreApplication::applicationDirPath() + "/benchmark.cache");
    if (!file.open(QFile::WriteOnly))
      return;

    file.write(JSON::generate(cache));
  }
}


//void Benchmark::run()
//{
//  qDebug() << "run" << currentThread();
//  m_settings = new AbstractSettings(QCoreApplication::applicationDirPath() + "/benchmark.conf");
//  m_settings->setInt("ConnectInterval", m_connectInterval);
//  m_settings->setInt("UsersCount",      m_usersCount);
//  m_settings->setString("NickPrefix",   m_nickPrefix);
//  m_settings->setString("Network",      m_serverAddr);

//  m_network = new Network();

//  m_settings->read();
//  m_connectInterval = m_settings->getInt("ConnectInterval");
//  m_usersCount      = m_settings->getInt("UsersCount");
//  m_nickPrefix      = m_settings->getString("NickPrefix");
//  m_serverAddr      = m_settings->getString("Network");

//  m_network->fromString(m_serverAddr);

//  connectToHost();

//  exec();
//}
