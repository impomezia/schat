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
#include <QProcess>
#include <QSound>
#include <QTimer>

#include "alerts/AlertType.h"
#include "alerts/MessageAlert.h"
#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "DateTime.h"
#include "messages/Message.h"
#include "net/SimpleID.h"
#include "Path.h"
#include "QtEscape.h"
#include "sglobal.h"
#include "ui/TabWidget.h"
#include "WebBridge.h"

ChatAlerts *ChatAlerts::m_self = 0;
int ChatAlerts::m_alerts = 0;
QMap<QByteArray, QList<QByteArray> > ChatAlerts::m_count;
QList<QByteArray> ChatAlerts::m_channels;

Alert::Alert(const QString &type)
  : m_id(ChatCore::randomId())
  , m_date(DateTime::utc())
  , m_type(type)
{
}


Alert::Alert(const QString &type, const QByteArray &id, qint64 date)
  : m_id(id)
  , m_date(date)
  , m_type(type)
{
}


Alert::Alert(const QString &type, qint64 date)
  : m_id(ChatCore::randomId())
  , m_date(date)
  , m_type(type)
{
}


void Alert::setTab(const QByteArray &sender, const QByteArray &dest)
{
  setTab(Message::detectTab(sender, dest));
}


class OnlineAlertType : public AlertType
{
public:
  inline OnlineAlertType(int weight)
  : AlertType(LS("online"), weight)
  {
    m_defaults[ALERT_POPUP_KEY] = false;
    m_defaults[ALERT_SOUND_KEY] = false;
    m_defaults[ALERT_FILE_KEY]  = QString();
    m_icon = QIcon(LS(":/images/online.png"));
  }

  inline QString name() const { return QObject::tr("Online"); }
};


class OfflineAlertType : public AlertType
{
public:
  inline OfflineAlertType(int weight)
  : AlertType(LS("offline"), weight)
  {
    m_defaults[ALERT_POPUP_KEY] = false;
    m_defaults[ALERT_SOUND_KEY] = false;
    m_defaults[ALERT_FILE_KEY]  = QString();
    m_icon = QIcon(LS(":/images/offline.png"));
  }

  inline QString name() const { return QObject::tr("Offline"); }
};


ChatAlerts::ChatAlerts(QObject *parent)
  : QObject(parent)
  , m_settings(ChatCore::settings())
{
  m_self = this;

  m_popup     = m_settings->setDefaultAndRead(ChatSettings::kAlertsPopup, true).toBool();
  m_popupDnD  = m_settings->setDefaultAndRead(LS("Alerts/Popup.DnD"),     false).toBool();
  m_sounds    = m_settings->setDefaultAndRead(LS("Alerts/Sounds"),        true).toBool();
  m_soundsDnD = m_settings->setDefaultAndRead(LS("Alerts/Sounds.DnD"),    false).toBool();

  add(new MessageAlertType(LS("public"),    100));
  add(new MessageAlertType(LS("private"),   200));
  add(new MessageAlertType(LS("referring"), 300));
  add(new OnlineAlertType(1000));
  add(new OfflineAlertType(1100));

  connect(ChatClient::i(), SIGNAL(offline()), SLOT(offline()));
  connect(ChatClient::i(), SIGNAL(ready()), SLOT(online()));
  connect(m_settings, SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));

  QTimer::singleShot(0, this, SLOT(loadSounds()));
}


ChatAlerts::~ChatAlerts()
{
  qDeleteAll(m_types);

  reset();
}


/*!
 * Добавление типа уведомления и загрузка его настроек.
 */
bool ChatAlerts::add(AlertType *type)
{
  if (!type || m_self->m_types.contains(type->type()))
    return false;

  m_self->m_types[type->type()] = type;

  const QString prefix = LS("Alerts/") + type->type() + LC('.');
  QMapIterator<QString, QVariant> i(type->defaults());
  while (i.hasNext()) {
    i.next();
    m_self->m_settings->setDefault(prefix + i.key(), i.value());
    type->options[i.key()] = m_self->m_settings->value(prefix + i.key());
  }

  return true;
}


/*!
 * \return \b true если звук отключен.
 */
bool ChatAlerts::isMute()
{
  if (ChatCore::isReady() && ChatClient::channel()->status() == Status::DnD)
    return !m_self->m_soundsDnD;

  return !m_self->m_sounds;
}


/*!
 * \return \b true если разрешены всплывающие окна.
 */
bool ChatAlerts::isPopup()
{
  if (!m_self->m_popup)
    return false;

  if (ChatClient::channel()->status() == Status::DnD)
    return m_self->m_popupDnD;

  return true;
}


/*!
 * Запуск уведомления.
 */
bool ChatAlerts::start(const Alert &alert)
{
  AlertType *type = m_self->m_types.value(alert.type());
  if (!type)
    return false;

  if (type->tray()) {
    const QByteArray &tabId = alert.tab();
    if (TabWidget::isActive(tabId))
      return false;

    int idType = SimpleID::typeOf(tabId);
    if (idType == SimpleID::ChannelId || idType == SimpleID::UserId) {
      ChannelBaseTab *tab = TabWidget::i()->channelTab(tabId, false, false);
      if (!tab)
        return false;

      add(tabId, alert.id());
    }
    else
      return false;
  }

  if (type->sound() && !isMute())
    play(type->value(LS("file")).toString());

  emit m_self->alert(alert);

  if (type->popup() && isPopup())
    emit m_self->popup(alert);

  return true;
}


QList<AlertType*> ChatAlerts::types()
{
  QMap<int, AlertType*> map;

  foreach (AlertType *type, m_self->m_types) {
    map[type->weight()] = type;
  }

  return map.values();
}


void ChatAlerts::play(const QString &file)
{
  if (!m_self->m_soundMap.contains(file) || m_self->m_soundQueue.contains(file))
    return;

  m_self->m_soundQueue.enqueue(file);
  if (m_self->m_soundQueue.size() == 1)
    QTimer::singleShot(0, m_self, SLOT(playSound()));
}


/*!
 * Удаление глобального уведомления для канала.
 *
 * \param channelId идентификатор канала.
 */
void ChatAlerts::remove(const QByteArray &channelId)
{
  QList<QByteArray> count = m_count.value(channelId);
  if (count.isEmpty())
    return;

  m_channels.removeAll(channelId);
  m_count.remove(channelId);
  m_alerts -= count.size();

  if (m_channels.isEmpty())
    emit m_self->alert(false);

  emit m_self->countChanged(m_alerts, count.size(), channelId);

  foreach (const QByteArray &alertId, count) {
    emit m_self->removed(alertId);
  }
}


void ChatAlerts::remove(const QByteArray &channelId, const QByteArray &alertId)
{
  QList<QByteArray> &count = m_count[channelId];
  if (!count.size() || !count.contains(alertId))
    return;

  if (count.size() > 1) {
    count.removeAll(alertId);
    m_alerts--;
    emit m_self->countChanged(m_alerts, count.size(), channelId);
    emit m_self->removed(alertId);
  }
  else
    remove(channelId);
}


void ChatAlerts::reset()
{
  m_alerts = 0;
  m_channels.clear();
  m_count.clear();
}


/*!
 * Заполение таблицы звуков.
 */
void ChatAlerts::loadSounds()
{
  QStringList paths(Path::data(Path::SystemScope));
  if (!Path::isPortable())
    paths.append(Path::data());

  foreach (QString path, paths) {
    QDir dir(path + LS("/sounds"));
    foreach (QFileInfo info, dir.entryInfoList(QStringList(LS("*.wav")), QDir::Files)) {
      m_soundMap[info.fileName()] = info.absoluteFilePath();
    }
  }
}


void ChatAlerts::offline()
{
  Alert alert(LS("offline"));

  QVariantMap popup;
  popup[LS("title")] = QString(LS("<b>%1</b>")).arg(tr("Offline"));
  popup[LS("text")]  = tr("Connection lost");
  alert.data()[LS("popup")] = popup;

  start(alert);
}


void ChatAlerts::online()
{
  Alert alert(LS("online"), ChatClient::io()->date());

  QString text = tr("Successfully connected to <b>%1</b>").arg(Qt::escape(ChatClient::serverName()));
  const QString peer = WebBridge::i()->serverPeer();
  if (!peer.isEmpty())
    text += LS(" (") + peer + LS(")");

  QVariantMap popup;
  popup[LS("title")] = QString(LS("<b>%1</b>")).arg(tr("Online"));
  popup[LS("text")]  = text;
  alert.data()[LS("popup")] = popup;

  start(alert);
}


void ChatAlerts::playSound()
{
  while (!m_soundQueue.isEmpty()) {
#   if defined(Q_OS_LINUX)
    QProcess::startDetached(QString(LS("aplay -q -N %1")).arg(m_soundMap.value(m_soundQueue.dequeue())));
#   else
    QSound::play(m_soundMap.value(m_soundQueue.dequeue()));
#   endif
  }
}


void ChatAlerts::settingsChanged(const QString &key, const QVariant &value)
{
  if (!key.startsWith(LS("Alerts/")))
    return;

  if (key == LS("Alerts/Sounds")) {
    m_sounds = value.toBool();
  }
  else if (key == LS("Alerts/Sounds.DnD")) {
    m_soundsDnD = value.toBool();
  }
  else if (key == ChatSettings::kAlertsPopup) {
    m_popup = value.toBool();
  }
  else if (key == LS("Alerts/Popup.DnD")) {
    m_popupDnD = value.toBool();
  }
  else {
    const QStringList detect = key.mid(7).split(LC('.'));
    if (detect.size() != 2)
      return;

    AlertType *type = m_types.value(detect.at(0));
    if (!type)
      return;

    type->options[detect.at(1)] = value;
  }
}


/*!
 * Добавление глобального уведомления для канала.
 *
 * \param channelId идентификатор канала.
 * \param alertId   идентификатор уведомления.
 */
void ChatAlerts::add(const QByteArray &channelId, const QByteArray &alertId)
{
  m_channels.removeAll(channelId);
  m_channels.prepend(channelId);

  QList<QByteArray> &count = m_count[channelId];
  if (count.contains(alertId))
    return;

  count.append(alertId);
  m_alerts++;

  if (m_channels.size() == 1)
    emit m_self->alert(true);

  emit m_self->countChanged(m_alerts, count.size(), channelId);
}
