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

#include <QFile>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QVariant>

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
    DownloadUpdate, ///< Закачка файла обновления.
    Paused          ///< Загрузка остановлена.
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
  void downloadProgress();
  void finished();
  void notify(const Notify &notify);
  void online();
  void readyRead();
  void restart();
  void start();
  void startDownload();

private:
  void checkUpdate();
  void readJSON();
  void setDone(Status status);

  ChatSettings *m_settings;        ///< Настройки чата.
  const QString m_prefix;          ///< Префикс настроек.
  DownloadState m_state;           ///< Состояние закачки.
  QBasicTimer *m_timer;            ///< Таймер периодической проверки.
  QByteArray m_rawJSON;            ///< Сырые JSON данные.
  QCryptographicHash *m_sha1;      ///< Класс для проверки SHA1 хеша файла.
  QFile m_file;                    ///< Файл обновлений.
  qint64 m_lastCheck;              ///< Время последней успешной проверки обновлений.
  QNetworkAccessManager m_manager; ///< Менеджер доступа к сети.
  QNetworkReply *m_current;        ///< Текущий ответ за запрос скачивания файла.
  Status m_status;                 ///< Статус проверки обновлений.
  UpdateInfo m_info;               ///< Информация об обновлении.
};

#endif /* UPDATEPLUGIN_P_H_ */
