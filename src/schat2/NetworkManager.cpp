/* $Id: NetworkManager.cpp 3717 2013-06-29 01:59:09Z IMPOMEZIA $
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

#include <QFile>
#include <QDir>

#include "Account.h"
#include "ChatCore.h"
#include "ChatHooks.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "net/packets/auth.h"
#include "net/SimpleID.h"
#include "NetworkManager.h"
#include "Path.h"
#include "sglobal.h"

NetworkItem::NetworkItem(const QByteArray &id)
  : m_id(id)
{
  if (id.size() > 35)
    m_provider = id.mid(35);
}


bool NetworkItem::isValid() const
{
  if (m_id.isEmpty() || m_userId.isEmpty() || m_cookie.isEmpty() || m_url.isEmpty() || m_name.isEmpty())
    return false;

  if (SimpleID::typeOf(NetworkManager::decode(m_id)) != SimpleID::ServerId)
    return false;

  if (SimpleID::typeOf(m_cookie) != SimpleID::CookieId)
    return false;

  if (SimpleID::typeOf(m_userId) != SimpleID::UserId)
    return false;

  return true;
}


/*!
 * Создание итема на основе текущего активного подключения.
 *
 * \param id Base32 кодированный идентификатор сервера с именем провайдера.
 */
NetworkItem* NetworkItem::item(const QByteArray &id)
{
  SimpleClient *client = ChatClient::io();
  NetworkItem *item = new NetworkItem(id);

  item->m_name   = ChatClient::serverName();
  item->m_url    = client->url().toString();
  item->m_cookie = client->cookie().toByteArray();
  item->m_userId = client->channelId();

  return item;
}


/*!
 * Чтение данных сети.
 */
void NetworkItem::read()
{
  ChatSettings *settings = ChatCore::settings();

  setAuth(settings->value(m_id + LS("/Auth")).toString());
  m_name = settings->value(m_id + LS("/Name")).toString();
  m_url  = settings->value(m_id + LS("/Url")).toString();
}


/*!
 * Запись данных сети.
 */
void NetworkItem::write()
{
  ChatSettings *settings = ChatCore::settings();

  settings->setValue(m_id + LS("/Auth"), auth(), false, true);
  settings->setValue(m_id + LS("/Name"), m_name, false, true);
  settings->setValue(m_id + LS("/Url"),  m_url, false, true);
}


/*!
 * Формирование авторизационной строки.
 */
QString NetworkItem::auth()
{
  if (!isValid())
    return QString();

  return ChatId::toBase32(NetworkManager::decode(m_id) + m_userId + m_cookie);
}


/*!
 * Чтение авторизационной строки.
 */
void NetworkItem::setAuth(const QString &auth)
{
  QByteArray data = ChatId::fromBase32(auth.toLatin1());
  if (data.size() != SimpleID::DefaultSize * 3)
    return;

  m_userId = data.mid(SimpleID::DefaultSize, SimpleID::DefaultSize);
  m_cookie = data.mid(SimpleID::DefaultSize * 2, SimpleID::DefaultSize);
}


NetworkManager::NetworkManager(QObject *parent)
  : QObject(parent)
  , m_invalids(0)
{
  load();

  new Hooks::Networks(this);

  connect(ChatClient::io(), SIGNAL(clientStateChanged(int,int)), SLOT(clientStateChanged(int)));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
}


bool NetworkManager::isAutoConnect() const
{
  if (ChatClient::state() != ChatClient::Offline)
    return false;

  if (m_invalids)
    return false;

  if (!ChatCore::settings()->value(LS("AutoConnect")).toBool())
    return false;

  if (ChatClient::channel()->status().value() == Status::Offline)
    return false;

  if (m_networks.data.isEmpty())
    return false;

  return true;
}


bool NetworkManager::open()
{
  if (isAutoConnect())
    return open(m_networks.first());

  return false;
}


/*!
 * Открытие нового соединения, используя идентификатор сервера.
 *
 * \param id Сохранённый идентификатор сервера.
 * \return true в случае успеха.
 */
bool NetworkManager::open(const QByteArray &id)
{
  if (!m_items.contains(id))
    return false;

  Network item = m_items.value(id);
  ChatClient::io()->setAuthType(AuthRequest::Cookie);
  return ChatClient::io()->openUrl(item->url(), item->cookie());
}


/*!
 * Возвращает состояние текущего выбранного итема.
 *
 * \return Возвращаемые значения:
 * - 0 Подключение не ассоциировано с выбранным итемом.
 * - 1 Подключение активно для текущего итема.
 * - 2 Идёт подключение.
 */
int NetworkManager::isSelectedActive() const
{
  if (ChatClient::state() == ChatClient::Online && m_selected == encode(ChatClient::serverId(), ChatClient::channel()->account()->provider))
    return 1;

  if (ChatClient::state() == ChatClient::Connecting && ChatClient::io()->url().toString() == item(m_selected)->url())
    return 2;

  return 0;
}


Network NetworkManager::item(const QByteArray &id) const
{
  if (!m_items.contains(id))
    return m_items.value(m_tmpId);

  return m_items.value(id);
}


/*!
 * Получение списка идентификаторов сетей, сохранённых в настройках.
 */
QList<Network> NetworkManager::items() const
{
  QList<Network> out;

  for (int i = 0; i < m_networks.data.size(); ++i) {
    Network item = m_items.value(m_networks.data.at(i));
    if (item->isValid())
      out.append(item);
  }

  return out;
}


/*!
 * Возвращает и при необходимости создаёт путь для хранения файлов сервера.
 *
 * \param id Base32 кодированный идентификатор сервера с опциональным именем авторизационного провайдера.
 */
QString NetworkManager::root(const QByteArray &id) const
{
  const ChatId _id(id.left(ChatId::kEncodedSize));
  if (_id.isNull())
    return QString();

  const QString out = Path::cache() + LC('/') + _id.toString();
  if (!QFile::exists(out))
    QDir().mkpath(out);

  return out;
}


/*!
 * Удаление сервера.
 */
void NetworkManager::removeItem(const QByteArray &id)
{
  if (id == m_tmpId)
    return;

  m_networks.data.removeAll(id);
  m_networks.write();
  ChatCore::settings()->remove(SimpleID::encode(id));

  if (id == ChatClient::serverId() && ChatClient::state() != ChatClient::Offline)
    ChatClient::io()->leave();
}


void NetworkManager::setSelected(const QByteArray &id)
{
  if (m_selected == id)
    return;

  m_selected = id;
  ChatNotify::start(Notify::NetworkSelected, id);
}


/*!
 * Получение не кодированного идентификатора из кодированного идентификатора и имени провайдера.
 */
QByteArray NetworkManager::decode(const QByteArray &id)
{
  return SimpleID::decode(id.left(34));
}


/*!
 * Кодирование идентификатора сервера и провайдера в строку, состоящую из Base32 кодированного идентификатора сервера
 * и имени провайдера, разделённых точкой. Например: "ZU7QZBNRLDAIUKYRGRSJSGAQZ4WN7Q4HKM.facebook"
 */
QByteArray NetworkManager::encode(const QByteArray &id, const QString &provider)
{
  QByteArray out = SimpleID::encode(id);
  if (!provider.isEmpty())
    out += '.' + provider.toLatin1();

  return out;
}


void NetworkManager::clientStateChanged(int state)
{
  if (state != ChatClient::Online)
    return;

  write();
}


void NetworkManager::notify(const Notify &notify)
{
  if (notify.type() == Notify::ServerRenamed) {
    Network item = this->item(ChatClient::serverId());
    if (!item)
      return;

    item->setName(ChatClient::serverName());
    item->write();
  }
}


/*!
 * Формирование таблицы серверов при запуске.
 *
 * Список серверов читается из настройки Networks, если сервер прошёл
 * проверку на валидность то он добавляется в таблицу \p m_items.
 */
void NetworkManager::load()
{
  m_networks.read();

  m_tmpId = SimpleID::encode(SimpleID::make("", SimpleID::ServerId));
  Network item(new NetworkItem(m_tmpId));
  item->setUrl(LS("schat://"));
  m_items[m_tmpId] = item;

  if (m_networks.data.isEmpty())
    return;

  QList<QByteArray> invalids;
  Settings settings(Path::config(), this);

  // Чтение данных серверов.
  for (int i = 0; i < m_networks.data.size(); ++i) {
    QByteArray id = m_networks.data.at(i);

    Network item(new NetworkItem(id));
    item->read();

    if (!item->isValid()) {
      invalids += id;
      continue;
    }

    m_items[id] = item;
  }

  // Удаление невалидных серверов.
  if (!invalids.isEmpty()) {
    for (int i = 0; i < invalids.size(); ++i) {
      m_networks.data.removeAll(invalids.at(i));
    }

    m_invalids = invalids.size();
    m_networks.write();
  }

  setSelected(m_networks.first());
}


/*!
 * Запись информации о новом сервере.
 */
void NetworkManager::write()
{
  m_selected = encode(ChatClient::serverId(), ChatClient::channel()->account()->provider);
  Network item(NetworkItem::item(m_selected));
  item->write();

  m_networks.data.removeAll(m_selected);
  m_networks.data.prepend(m_selected);
  m_networks.write();

  m_items[m_selected] = item;
  const QString dir = root(m_selected);

# if !defined(SCHAT_NO_SSL)
  QList<QSslCertificate> chain = ChatClient::io()->peerCertificateChain();
  if (chain.size()) {
    QFile file(dir + LS("/peer.crt"));
    if (file.open(QFile::WriteOnly)) {
      foreach (const QSslCertificate &cert, chain) {
        file.write(cert.toPem());
      }
    }
  }
# endif

  ChatNotify::start(Notify::NetworkChanged, m_selected);
}


NetworkManager::Networks::Networks()
{
}


/*!
 * Получение идентификатора первого сервера в списке или пустого
 * идентификатора если список пуст.
 */
QByteArray NetworkManager::Networks::first()
{
  if (data.isEmpty())
    return QByteArray();

  return data.at(0);
}


/*!
 * Загрузка списка сетей из настроек.
 */
void NetworkManager::Networks::read()
{
  data.clear();
  QStringList networks = ChatCore::settings()->value(LS("Networks")).toStringList();

  foreach (QString network, networks) {
    QByteArray id = decode(network.toLatin1());
    if (SimpleID::typeOf(id) == SimpleID::ServerId)
      data += network.toLatin1();
  }
}


/*!
 * Запись списка сетей в настройки.
 */
void NetworkManager::Networks::write()
{
  QStringList out;
  foreach (QByteArray id, data) {
    out += id;
  }

  ChatCore::settings()->setValue(LS("Networks"), out);
}
