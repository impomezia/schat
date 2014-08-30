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

#include <QDir>
#include <QCoreApplication>
#include <QTimer>
#include <QUrl>

#include "AuthCore.h"
#include "AuthHandler.h"
#include "AuthState.h"
#include "HandlerRoute.h"
#include "handlers.h"
#include "NodeLog.h"
#include "oauth2/facebook/FacebookAuthData.h"
#include "oauth2/github/GithubAuthData.h"
#include "oauth2/google/GoogleAuthData.h"
#include "oauth2/live/LiveAuthData.h"
#include "oauth2/mail_ru/MailRuAuthData.h"
#include "oauth2/OAuthData.h"
#include "oauth2/odnoklassniki/OdnoklassnikiAuthData.h"
#include "oauth2/simpleid/SimpleIdAuthData.h"
#include "oauth2/stackoverflow/StackOverflowAuthData.h"
#include "oauth2/vkontakte/VkontakteAuthData.h"
#include "oauth2/yandex/YandexAuthData.h"
#include "Path.h"
#include "Settings.h"
#include "sglobal.h"
#include "Storage.h"
#include "tools/CmdLine.h"
#include "Tufao/httpserver.h"
#include "version.h"

AuthCore *AuthCore::m_self = 0;

#define LOG_A1010 LOG_FATAL("A1010", "AuthCore", "Configuration option \"BaseUrl\" is not set")
#define LOG_A1011 LOG_FATAL("A1011", "AuthCore", "Providers list is empty")
#define LOG_A1012 LOG_FATAL("A1012", "AuthCore", "Failed to open ports for incoming connections")
#define LOG_A1013  LOG_INFO("A1013", "AuthCore", "Server successfully started")
#define LOG_A1014  LOG_INFO("A1014", "AuthCore", "Added url: " << url.toString())
#define LOG_A1015 LOG_ERROR("A1015", "AuthCore", "Failed to open port for incoming connections: " << url.toString())
#define LOG_A1016  LOG_INFO("A1016", "AuthCore", "Added authorization provider: " << name)

AuthCore::AuthCore(QObject *parent)
  : QObject(parent)
{
  m_self = this;
  Path::init();

  CmdLine::createPid(Path::app());

  m_log = new NodeLog();

  m_settings = new Settings(defaultConf(), this);
  m_settings->setDefault(LS("BaseUrl"),   QString());
  m_settings->setDefault(LS("Listen"),    QStringList(LS("http://0.0.0.0:7668")));
  m_settings->setDefault(LS("LogColor"),  true);
  m_settings->setDefault(LS("LogLevel"),  NodeLog::WarnLevel);
  m_settings->setDefault(LS("LogOutput"), NodeLog::FileOut);
  m_settings->setDefault(LS("Order"),     QStringList() << LS("facebook") << LS("vkontakte") << LS("google") << LS("live") << LS("github") << LS("stackoverflow") << LS("yandex") << LS("odnoklassniki") << LS("mail_ru") << LS("simpleid"));
  m_settings->setDefault(LS("Root"),      defaultRoot());

  openLog();

  m_handler = new AuthHandler(this);
  m_handler->setRoot(QDir::cleanPath(m_settings->value(LS("Root")).toString()));

  m_state = new AuthState(this);

  add(new ProvidersHandler());
  add(new StateHandlerCreator());
  add(new FacebookAuthCreator());
  add(new GoogleAuthCreator());
  add(new YandexAuthCreator());
  add(new MailRuAuthCreator());
  add(new VkontakteAuthCreator());
  add(new OdnoklassnikiAuthCreator());
  add(new LiveAuthCreator());
  add(new GithubAuthCreator());
  add(new StackOverflowAuthCreator());
  add(new SimpleIdAuthCreator());

  QTimer::singleShot(0, this, SLOT(start()));
}


AuthCore::~AuthCore()
{
  delete m_log;
  qDeleteAll(m_providers);

  CmdLine::removePid(Path::app());
}


/*!
 * Возвращает корневую директорию для файлов доступных по HTTP.
 */
QString AuthCore::root()
{
  return m_self->m_handler->root();
}


void AuthCore::version()
{
  QTextStream out(stdout);
  out << LS("Simple Chat Auth Daemon, version ") << SCHAT_VERSION;
  if (SCHAT_VER_PATH)
    out << LC('.') << SCHAT_VER_PATH;

  out << endl << endl;
  out << SCHAT_COPYRIGHT << LS(", https://schat.me") << endl;
}


void AuthCore::start()
{
  m_baseUrl = m_settings->value(LS("BaseUrl")).toByteArray();
  if (m_baseUrl.isEmpty()) {
    LOG_A1010
    QCoreApplication::exit(1);
    return;
  }

  add(new FacebookAuthData());
  add(new GoogleAuthData());
  add(new YandexAuthData());
  add(new VkontakteAuthData());
  add(new MailRuAuthData());
  add(new OdnoklassnikiAuthData());
  add(new LiveAuthData());
  add(new GithubAuthData());
  add(new StackOverflowAuthData());
  add(new SimpleIdAuthData());

  if (m_providers.isEmpty()) {
    LOG_A1011

    QCoreApplication::exit(2);
    return;
  }

  QStringList listen = m_settings->value(LS("Listen")).toStringList();
  foreach (const QString &url, listen) {
    add(QUrl(url));
  }

  if (m_servers.isEmpty()) {
    LOG_A1012

    QCoreApplication::exit(3);
    return;
  }

  LOG_A1013
}


QString AuthCore::defaultConf() const
{
# if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
  if (!Path::isPortable())
    return LS("/etc/schatd2/") + Path::app() + LS(".conf");
# endif

  return Path::data() + LC('/') + Path::app() + LS(".conf");
}


QString AuthCore::defaultRoot() const
{
# if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
  if (!Path::isPortable())
    return SCHAT_PREFIX + LS("/share/schatd2/www");
# endif

  return Path::data(Path::SystemScope) + LS("/www");
}


/*!
 * Добавление адреса, на котором сервер будет принимать подключения.
 */
void AuthCore::add(const QUrl &url)
{
  Tufao::HttpServer *server = new Tufao::HttpServer(this);
  connect(server, SIGNAL(requestReady(Tufao::HttpServerRequest*,Tufao::HttpServerResponse*)), m_handler, SLOT(handleRequest(Tufao::HttpServerRequest*,Tufao::HttpServerResponse*)));

  if (server->listen(QHostAddress(url.host()), url.port())) {
    LOG_A1014
    m_servers.append(server);
  }
  else {
    LOG_A1015
    server->deleteLater();
  }
}


void AuthCore::add(HandlerCreator *handler)
{
  HandlerRoute::routes.append(handler);
}


void AuthCore::add(OAuthData *data)
{
  const QByteArray &name = data->provider;

  if (!m_providers.contains(name) && data->read()) {
    m_providers[name] = data;
    LOG_A1016
  }
  else
    delete data;
}


void AuthCore::openLog()
{
  QString path = Path::cache();
# if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
  if (!Path::isPortable())
    path = LS("/var/log/schatd2");
# endif

  m_log->open(path + LC('/') + Path::app() + LS(".log"), static_cast<NodeLog::Level>(m_settings->value(LS("LogLevel")).toInt()));
  m_log->setOutFlags(static_cast<NodeLog::OutFlags>(m_settings->value(LS("LogOutput")).toInt()));
  m_log->setColors(m_settings->value(LS("LogColor")).toBool());
}
