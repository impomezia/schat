/* $Id: ChatSettings.h 3603 2013-03-27 15:19:21Z IMPOMEZIA $
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

#ifndef CHATSETTINGS_H_
#define CHATSETTINGS_H_

#include <QStringList>

#include "Settings.h"

class FeedNotify;
class Notify;

#define SETTINGS_AUTO_AWAY               QLatin1String("AutoAway")
#define SETTINGS_AUTO_CONNECT            QLatin1String("AutoConnect")
#define SETTINGS_AUTO_JOIN               QLatin1String("AutoJoin")
#define SETTINGS_AUTO_SCROLL             QLatin1String("AutoScroll")
#define SETTINGS_CHANNELS_IGNORING       QLatin1String("Channels/Ignoring")
#define SETTINGS_DEFAULT_SERVER          QLatin1String("DefaultServer")
#define SETTINGS_DISPLAY_SECONDS         QLatin1String("Display/Seconds")
#define SETTINGS_DISPLAY_SERVICE         QLatin1String("Display/Service")
#define SETTINGS_HISTORY_AUTO_LOAD       QLatin1String("History/AutoLoad")
#define SETTINGS_LABS_DEVELOPER_EXTRAS   QLatin1String("Labs/DeveloperExtras")
#define SETTINGS_LABS_DISABLE_UI         QLatin1String("Labs/DisableUI")
#define SETTINGS_LABS_STATIC_TRAY_ALERTS QLatin1String("Labs/StaticTrayAlerts")
#define SETTINGS_MAXIMIZED               QLatin1String("Maximized")
#define SETTINGS_NETWORKS                QLatin1String("Networks")
#define SETTINGS_PINNED_TABS             QLatin1String("PinnedTabs")
#define SETTINGS_PROFILE_GENDER          QLatin1String("Profile/Gender")
#define SETTINGS_PROFILE_NICK            QLatin1String("Profile/Nick")
#define SETTINGS_PROFILE_STATUS          QLatin1String("Profile/Status")
#define SETTINGS_TOOLBAR_ACTIONS         QLatin1String("ToolBarActions")
#define SETTINGS_TRANSLATION             QLatin1String("Translation")
#define SETTINGS_WINDOWS_AERO            QLatin1String("WindowsAero")

class SCHAT_CORE_EXPORT ChatSettings : public Settings
{
  Q_OBJECT

public:
  ChatSettings(const QString &fileName, const QString &defaultFile, QObject *parent = 0);
  inline bool isSynced() const { return m_synced; }
  QVariant setDefaultAndRead(const QString &key, const QVariant &value);
  void init();
  void setDefault(const QString &key, const QVariant &value);
  void setLocalDefault(const QString &key, const QVariant &value);
  void setValue(const QString &key, const QVariant &value, bool notify = true, bool local = false);

signals:
  void synced();

private slots:
  void notify(const Notify &notify);
  void offline();
  void ready();

private:
  void set(bool offline);
  void set(const QString &key, const QVariant &value);

  bool m_synced;         ///< \b true если настройки сихронизированы с сервером.
  QSettings *m_settings; ///< Альтернативные настройки по умолчанию.
  QStringList m_local;   ///< Список ключей локальных настроек.
};

#endif /* CHATSETTINGS_H_ */
