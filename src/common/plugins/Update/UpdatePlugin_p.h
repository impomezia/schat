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

#ifndef UPDATEPLUGIN_P_H_
#define UPDATEPLUGIN_P_H_

#include <QUrl>
#include <QVariant>

#include "interfaces/IDownloadItem.h"
#include "plugins/ChatPlugin.h"

class ChatSettings;
class Notify;
class QBasicTimer;
class QCryptographicHash;
class QMouseEvent;
class QNetworkReply;

/*!
 * Информация об обновлении.
 */
class UpdateInfo
{
public:
  inline UpdateInfo() : revision(0), size(0) {}
  UpdateInfo(const QUrl &url);
  UpdateInfo(const QVariantMap &data);
  bool isValid() const;

  int revision;    ///< SVN ревизия.
  int size;        ///< Размер обновлений.
  QByteArray hash; ///< SHA1 контрольная сумма обновлений.
  QString page;    ///< Страница загрузки обновления.
  QString version; ///< Доступная версия.
  QUrl notes;      ///< Адрес страницы с информацией о версии.
  QUrl url;        ///< Адрес источинка обновлений или адрес загружаемого файла.
};


/*!
 * Реализация плагина \b Update.
 */
class UpdatePluginImpl : public ChatPlugin
{
  Q_OBJECT

public:
  /// Состояние закачки.
  enum DownloadState {
    Idle,           ///< Нет активной закачки.
    DownloadJSON,   ///< Закачка JSON файла с информацией об обновлении.
    DownloadUpdate  ///< Закачка файла обновления.
  };

  /// Статус проверки обновлений.
  enum Status {
    Unknown,         ///< Обновления не проверялись.
    CheckError,      ///< Не удалось получить информацию об обновлениях.
    NoUpdates,       ///< Нет доступных обновлений, установлена последняя версия.
    UpdateAvailable, ///< Доступно обновление.
    DownloadError,   ///< Ошибка при скачивании обновления.
    UpdateReady      ///< Обновление скачано и готово к применению.
  };

  static const QString kPrefix;
  static const QString kUpdateAutoDownload;
  static const QString kUpdateAutoRemove;
  static const QString kUpdateChannel;
  static const QString kUpdateReady;
  static const QString kUpdateRevision;
  static const QString kUpdateUrl;
  static const QString kUpdateVersion;

  UpdatePluginImpl(QObject *parent);
  ~UpdatePluginImpl();
  inline const UpdateInfo &info() const { return m_info; }
  inline DownloadState state() const    { return m_state; }
  inline Status status() const          { return m_status; }
  static bool supportDownload();
  void chatReady() Q_DECL_OVERRIDE;

signals:
  void done(int status);

public slots:
  void check();

protected:
  void timerEvent(QTimerEvent *event);

private slots:
  void clicked(const QString &key, QMouseEvent *event);
  void download();
  void onDownloadProgress(DownloadItem item, qint64 bytesReceived, qint64 bytesTotal);
  void onFinished(const QByteArray &hash);
  void onFinished(DownloadItem item);
  void online();
  void onNotify(const Notify &notify);
  void restart();
  void start();

private:
  void readJSON(const QByteArray &raw);
  void setDone(Status status);

  ChatSettings *m_settings;        ///< Настройки чата.
  DownloadItem m_item;
  DownloadState m_state;           ///< Состояние закачки.
  QBasicTimer *m_timer;            ///< Таймер периодической проверки.
  qint64 m_lastCheck;              ///< Время последней успешной проверки обновлений.
  Status m_status;                 ///< Статус проверки обновлений.
  UpdateInfo m_info;               ///< Информация об обновлении.
};

#endif /* UPDATEPLUGIN_P_H_ */
