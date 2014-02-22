/* $Id: WebBridge.cpp 3754 2013-07-14 19:50:00Z IMPOMEZIA $
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

#include <QCoreApplication>
#include <QDate>
#include <QHostAddress>
#include <QLocale>
#include <QSslCipher>
#include <QSslConfiguration>

#include "acl/AclValue.h"
#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "ChatUrls.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientFeeds.h"
#include "client/SimpleClient.h"
#include "hooks/ChatViewHooks.h"
#include "hooks/FeedHook.h"
#include "JSON.h"
#include "net/SimpleID.h"
#include "NetworkManager.h"
#include "Profile.h"
#include "QtEscape.h"
#include "sglobal.h"
#include "Tr.h"
#include "Translation.h"
#include "ui/TabWidget.h"
#include "WebBridge.h"

WebBridge *WebBridge::m_self = 0;

class WebBridgeTr : public Tr
{
  Q_DECLARE_TR_FUNCTIONS(WebBridgeTr)

public:
  WebBridgeTr() : Tr() {}

protected:
  QString valueImpl(const QString &key) const
  {
    if (key == LS("my_computers"))           return tr("My Computers");
    else if (key == LS("ok"))                return tr("OK");
    else if (key == LS("my_computers_desc")) return tr("These are the computers currently linked to your account.");
    else if (key == LS("computer_name"))     return tr("Computer name");
    else if (key == LS("last_activity"))     return tr("Last Activity");
    else if (key == LS("actions"))           return tr("Actions");
    else if (key == LS("unlink"))            return tr("Unlink");
    else if (key == LS("version"))           return tr("<b>Version:</b>");
    else if (key == LS("last_ip"))           return tr("<b>Last IP Address:</b>");
    else if (key == LS("profile"))           return tr("Profile");
    else if (key == LS("connections"))       return tr("Connections");
    else if (key == LS("user_offline"))      return tr("User offline");
    else if (key == LS("was_online_m"))      return tr("Was online", "Male");
    else if (key == LS("was_online_f"))      return tr("Was online", "Female");
    else if (key == LS("chat_version"))      return tr("Version");
    else if (key == LS("os_name"))           return tr("OS");
    else if (key == LS("kB"))                return tr("kB");
    else if (key == LS("MB"))                return tr("MB");
    else if (key == LS("enabled"))           return tr("Enabled");
    else if (key == LS("enable"))            return tr("Enable");
    else if (key == LS("settings"))          return tr("Settings");
    else if (key == LS("restart_now"))       return tr("Restart now");
    else if (key == LS("will_be_enabled"))   return tr("✔ Plugin will be enabled after you restart Simple Chat.");
    else if (key == LS("will_be_disabled"))  return tr("Plugin will be disabled after you restart Simple Chat.");
    else if (key == LS("sign_in_with"))      return tr("Sign in using your account with");
    else if (key == LS("or_anon_connect"))   return tr("Or connect anonymously");
    else if (key == LS("anonymous_user"))    return tr("anonymous user");
    else if (key == LS("status_Online"))     return tr("Online");
    else if (key == LS("status_Away"))       return tr("Away");
    else if (key == LS("status_DnD"))        return tr("Do not disturb");
    else if (key == LS("status_FreeForChat"))return tr("Free for Chat");
    else if (key == LS("status_Offline"))    return tr("Offline");
    else if (key == LS("thanks"))            return tr("Thanks");
    else if (key == LS("loading"))           return tr("Loading...");
    else if (key == LS("activity"))          return tr("Activity");
    else if (key == LS("activity_sent"))     return tr("Sent:");
    else if (key == LS("activity_receved"))  return tr("Receved:");
    else if (key == LS("server_version"))    return tr("Server version:");
    else if (key == LS("encryption"))        return tr("Encryption:");
    else if (key == LS("no-encryption"))     return tr("no");
    else if (key == LS("lang"))              return tr("en");
    else if (key == LS("message-removed"))   return tr("This message has been removed.");
    return QString();
  }
};


WebBridge::WebBridge(QObject *parent)
  : QObject(parent)
{
  m_self = this;
  m_tr = new WebBridgeTr();
  retranslate();

  connect(ChatClient::channels(), SIGNAL(channel(ChannelInfo)), SLOT(channel(ChannelInfo)));
  connect(ChatClient::channels(), SIGNAL(quit(QByteArray)), SLOT(quit(QByteArray)));
  connect(ChatClient::i(), SIGNAL(online()), SIGNAL(online()));
  connect(ChatClient::i(), SIGNAL(offline()), SIGNAL(offline()));
  connect(ChatCore::settings(), SIGNAL(changed(QString,QVariant)), SIGNAL(settingsChanged(QString,QVariant)));
}


WebBridge::~WebBridge()
{
  delete m_tr;
}


bool WebBridge::isOnline() const
{
  return ChatClient::state() == ChatClient::Online;
}


int WebBridge::aclToInt(const QString &acl) const
{
  return AclValue::toInt(acl.toLatin1());
}


int WebBridge::match(const QString &channelId, const QString &userId) const
{
  ClientChannel channel = ChatClient::channels()->get(SimpleID::decode(channelId));
  if (!channel)
    return -1;

  return ClientFeeds::match(channel, ChatClient::channels()->get(SimpleID::decode(userId)));
}


qint64 WebBridge::mdate(const QString &id, const QString &name) const
{
  ClientChannel channel = ChatClient::channels()->get(SimpleID::decode(id));
  if (channel) {
    FeedPtr feed = channel->feed(name, false);
    if (feed)
      return feed->head().date();
  }

  return 0;
}


QString WebBridge::bytesToHuman(qint64 size, bool html) const
{
  QString num;
  QString key;
  if (size < 1000000) {
    if (!size)
      num = LS("0");
    else if (size && size < 1024)
      num = LS("1");
    else
      num = QString::number((int) size / 1024);

    key = LS("kB");
  }
  else {
    num = QString::number((double) size / 1048576, 'f', 2);
    key = LS("MB");
  }

  if (html)
    num += LS(" <span data-tr=\"") + key + LS("\">") + Tr::value(key) + LS("</span>");
  else
    num += LC(' ') + Tr::value(key);

  return num;
}


QVariant WebBridge::channel(const QString &id) const
{
  QVariantMap data = channel(SimpleID::decode(id.toLatin1()));
  if (data.isEmpty())
    return false;

  return data;
}


QString WebBridge::day(const QString &day) const
{
  QLocale locale(ChatCore::translation()->name());
  return locale.toString(QDate::fromString(day, LS("yyyy_MM_dd")));
}


QString WebBridge::id() const
{
  return SimpleID::encode(ChatClient::id());
}


QString WebBridge::randomId() const
{
  return SimpleID::encode(ChatCore::randomId());
}


QString WebBridge::serverId() const
{
  return SimpleID::encode(ChatClient::serverId());
}


/*!
 * Информация о реальном адресе сервера.
 *
 * - Если используется стандартный порт и реальный адрес сервера совпадает с именем сервера возвращается пустая строка.
 * - Если используется не стандартный порт возвращается адрес сервера и порт, в противном случае только адрес.
 */
QString WebBridge::serverPeer() const
{
  QString addr = ChatClient::io()->peerAddress().toString();
  qint16 port = ChatClient::io()->peerPort();

  if (port == 7667 && addr == ChatClient::serverName())
    return QString();

  if (port != 7667)
    addr += LC(':') + QString::number(port);

  return addr;
}


QString WebBridge::status(const QString &id) const
{
  if (ChatClient::state() != ChatClient::Online)
    return LS("Offline");

  ClientChannel channel = ChatClient::channels()->get(SimpleID::decode(id));
  if (!channel)
    return LS("Offline");

  return channel->status().toString();
}


QString WebBridge::statusText(int status) const
{
  return Notice::status(status);
}


QString WebBridge::toBase32(const QString &text)
{
  return ChatId::toBase32(text.toUtf8());
}


QString WebBridge::translate(const QString &key) const
{
  return Tr::value(key);
}


QStringList WebBridge::fields() const
{
  return Profile::fields();
}


QVariant WebBridge::encryption() const
{
# if !defined(SCHAT_NO_SSL)
  SimpleClient *io = ChatClient::io();
  if (!io->isEncrypted())
    return QVariant();

  QVariantMap data;
  data[LS("protocol")] = io->sslConfiguration().protocol();

  const QSslCertificate cert = io->peerCertificate();
# if QT_VERSION >= 0x050000
  QString cn;
  QStringList cns = cert.subjectInfo(QSslCertificate::CommonName);
  if (!cns.isEmpty())
    cn = cns.first();
# else
  data[LS("CN")] = cert.subjectInfo(QSslCertificate::CommonName);
# endif

  QSslCipher cipher = io->sessionCipher();
  data[LS("cipher")]       = cipher.name() + LC('/') + cipher.authenticationMethod();
  data[LS("daysToExpiry")] = QDateTime::currentDateTime().daysTo(cert.expiryDate());
  data[LS("url")]          = QString(QUrl::fromLocalFile(ChatCore::networks()->root(SimpleID::encode(ChatClient::serverId())) + LS("/peer.crt")).toEncoded());
  return data;
# else
  return QVariant();
# endif
}


/*!
 * Получение фида по идентификатору канала и имени.
 */
QVariant WebBridge::feed(const QString &id, const QString &name, int options) const
{
  ClientChannel channel = ChatClient::channels()->get(SimpleID::decode(id));
  if (!channel)
    return false;

  return feed(channel, name, options);
}


/*!
 * Получение собственного фида по имени.
 */
QVariant WebBridge::feed(const QString &name, int options) const
{
  return feed(ChatClient::channel(), name, options);
}


QVariant WebBridge::value(const QString &key) const
{
  return ChatCore::settings()->value(key);
}


QVariantMap WebBridge::traffic() const
{
  QVariantMap out;
  out[LS("tx")] = ChatClient::io()->tx();
  out[LS("rx")] = ChatClient::io()->rx();

  return out;
}


void WebBridge::get(const QString &id, const QString &name, const QVariantMap &json)
{
  ClientFeeds::request(ChatClient::channels()->get(SimpleID::decode(id)), FEED_METHOD_GET, name, json);
}


void WebBridge::pinTab(const QString &id) const
{
  TabWidget::pin(SimpleID::decode(id));
}


void WebBridge::request(const QString &id, const QString &command, const QString &name, const QVariantMap &json)
{
  ClientFeeds::request(SimpleID::decode(id.toLatin1()), command, name, json);
}


void WebBridge::setTabPage(const QString &id, int page)
{
  ClientChannel channel = ChatClient::channels()->get(SimpleID::decode(id.toLatin1()));
  if (!channel)
    return;

  channel->data()[LS("page")] = page;
}


QString WebBridge::toLink(const QByteArray &id, const QString &action, const QString &name)
{
  QVariantMap data = channel(id, action);
  if (data.isEmpty() && !name.isEmpty())
    data = channel(ClientChannel(new Channel(id, name)), action);

  if (data.isEmpty())
    return QString();

  return QString(LS("<a class=\"nick %1 color-%2\" href=\"%3\">%4</a>"))
      .arg(data.value(LS("Id")).toString())
      .arg(data.value(LS("Color")).toString())
      .arg(data.value(LS("Url")).toString())
      .arg(Qt::escape(data.value(LS("Name")).toString()));
}



/*!
 * Базовая функция получения фида.
 *
 * \param channel Канал владелец фида.
 * \param name    Имя фида.
 * \param options - 0 Если фид не найден в кэше он будет запрошен с сервера, иначе будет использоваться кэшированная копия.
 *                - 1 Запросить фид c сервера, с проверкой на модификацию фида, если фид не изменился его тело не будет передано и клиент будет использовать кэш.
 *                - 2 Форсированная загрузка фида.
 *                - 3 Отключить взаимодействие с сервером, если фид не найден в кэше вернуть false.
 *                - 4 Также как и 1, но тело фида будет сразу возвращено если оно есть в кеше.
 *
 * \return JSON тело фида или false, если фид не доступен локально.
 */
QVariant WebBridge::feed(ClientChannel channel, const QString &name, int options)
{
  if (ChatClient::state() != ChatClient::Online)
    options = 3;

  if (options == 0 || options == 3) {
    FeedPtr feed = channel->feed(name, false);
    if (feed)
      return feed->data();

    if (options == 0)
      ClientFeeds::request(channel->id(), FEED_METHOD_GET, name);
  }
  else if (options == 1)
    ClientFeeds::request(channel, FEED_METHOD_GET, name);
  else if (options == 2)
    ClientFeeds::request(channel->id(), FEED_METHOD_GET, name);
  else if (options == 4) {
    QVariant result = feed(channel, name, 3);
    feed(channel, name, 1);
    return result;
  }

  return false;
}


/*!
 * Получение информации о канале.
 */
QVariantMap WebBridge::channel(ClientChannel channel, const QString &action)
{
  if (!channel)
    return QVariantMap();

  QVariantMap data;
  data[LS("Id")]     = QString(SimpleID::encode(channel->id()));
  data[LS("Name")]   = channel->name();
  data[LS("Url")]    = ChatUrls::toUrl(channel, action).toString();
  data[LS("Color")]  = channel->gender().toString();
  data[LS("Status")] = channel->status().toString();
  data[LS("Gender")] = channel->gender().value();
  return data;
}


/*!
 * Получение информации о канале.
 */
QVariantMap WebBridge::channel(const QByteArray &id, const QString &action)
{
  return channel(ChatClient::channels()->get(id), action);
}


QVariantMap WebBridge::feed(const FeedNotify &notify)
{
  ClientChannel channel = ChatClient::channels()->get(notify.channel());
  if (!channel)
    return QVariantMap();

  const int type = notify.type();
  QVariantMap out;

  if (type == Notify::FeedData) {
    FeedPtr feed = channel->feed(notify.name(), false);
    if (!feed)
      return out;

    out[FeedNotify::kData] = feed->data();
    out[FeedNotify::kType] = FeedNotify::kTypeBody;
  }
  else {
    out[FeedNotify::kData] = notify.json();
    out[FeedNotify::kType] = FeedNotify::kTypeReply;
  }

  out[FeedNotify::kStatus] = notify.status();
  out[FeedNotify::kName]   = notify.name();
  out[FeedNotify::kOwn]    = notify.channel() == ChatClient::id();
  out[FeedNotify::kCmd]    = notify.command();
  out[FeedNotify::kFeed]   = notify.feed();
  out[FeedNotify::kPath]   = notify.path();
  out[FeedNotify::kId]     = ChatId(notify.channel()).toString();

  foreach (FeedHook *hook, m_self->m_feedHooks)
    hook->hook(notify, out);

  return out;
}


void WebBridge::retranslate()
{
  ChatViewHooks::retranslate();
  emit retranslated();
}


void WebBridge::channel(const ChannelInfo &info)
{
  QVariantMap data = WebBridge::channel(info.id());
  if (info.option() == ChannelInfo::New || info.option() == ChannelInfo::Renamed)
    emit renamed(data);

  emit recolored(data);
  emit statusChanged(data.value(LS("Id")).toString(), data.value(LS("Status")).toString());
}


void WebBridge::quit(const QByteArray &user)
{
  emit statusChanged(SimpleID::encode(user), LS("Offline"));
}
