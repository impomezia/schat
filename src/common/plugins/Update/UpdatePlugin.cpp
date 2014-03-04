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

#include <QApplication>
#include <QBasicTimer>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <QFileInfo>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressBar>
#include <QTimer>
#include <QtPlugin>
#include <QVBoxLayout>

#if QT_VERSION >= 0x050000
# include <QWebPage>
#else
# include <qwebkitversion.h>
#endif

#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "DateTime.h"
#include "JSON.h"
#include "Path.h"
#include "sglobal.h"
#include "tools/OsInfo.h"
#include "tools/Ver.h"
#include "Translation.h"
#include "ui/BgOperationWidget.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/AboutTab.h"
#include "ui/TabWidget.h"
#include "UpdatePlugin.h"
#include "UpdatePlugin_p.h"
#include "UpdateSettings.h"
#include "UpdateWidget.h"
#include "version.h"

#define SCHAT_UPDATE_LABEL(x) QString(LS("<a href='#' style='text-decoration:none; color:#216ea7;'>%1</a>")).arg(x)
#define SCHAT_UPDATE_INTERVAL 24 * 60 * 60 * 1000

UpdateInfo::UpdateInfo(const QUrl &url)
  : revision(0)
  , size(0)
  , url(url)
{
}


UpdateInfo::UpdateInfo(const QVariantMap &data)
  : revision(0)
  , size(0)
{
  if (data.isEmpty())
    return;

  version  = data.value(LS("version")).toString();
  revision = data.value(LS("revision")).toInt();
  url      = data.value(LS("file")).toUrl();
  size     = data.value(LS("size")).toInt();
  hash     = QByteArray::fromHex(data.value(LS("hash")).toByteArray());
  notes    = data.value(LS("notes")).toUrl();
  page     = data.value(LS("page")).toString();

  if (notes.isEmpty())
    notes = LS("http://wiki.schat.me/Simple_Chat_") + version;

  if (page.isEmpty())
    page = url.toString();

# if defined(Q_OS_WIN)
  if (!UpdatePluginImpl::supportDownload())
    size = 0;
# endif
}


/*!
 * Проверка корректности данных обновления.
 */
bool UpdateInfo::isValid() const
{
  if (version.isEmpty() || Ver(version) < LS("1.99.50"))
    return false;

  if (revision < 3361)
    return false;

  if (UpdatePluginImpl::supportDownload() && (!url.isValid() || size < 1 || hash.size() != 20))
    return false;

  return true;
}


UpdatePluginImpl::UpdatePluginImpl(QObject *parent)
  : ChatPlugin(parent)
  , m_settings(ChatCore::settings())
  , m_prefix(LS("Update"))
  , m_state(Idle)
  , m_lastCheck(0)
  , m_current(0)
  , m_status(Unknown)
{
  m_settings->setLocalDefault(m_prefix + LS("/Url"),          LS("https://download.schat.me/schat2/update.json"));
  m_settings->setLocalDefault(m_prefix + LS("/Channel"),      LS("stable"));
  m_settings->setLocalDefault(m_prefix + LS("/AutoDownload"), true);
  m_settings->setLocalDefault(m_prefix + LS("/Ready"),        false);
  m_settings->setLocalDefault(m_prefix + LS("/Version"),      QString());
  m_settings->setLocalDefault(m_prefix + LS("/Revision"),     0);

  ChatCore::translation()->addOther(LS("update"));

  m_timer = new QBasicTimer();
  m_timer->start(60 * 60 * 1000, this);
  m_sha1 = new QCryptographicHash(QCryptographicHash::Sha1);
}


UpdatePluginImpl::~UpdatePluginImpl()
{
  if (m_timer->isActive())
    m_timer->stop();

  delete m_timer;
  delete m_sha1;
}


bool UpdatePluginImpl::supportDownload()
{
# if defined(Q_OS_WIN)
  return Path::isPortable();
# else
  return false;
# endif
}


void UpdatePluginImpl::chatReady()
{
  QTimer::singleShot(0, this, SLOT(start()));
}


/*!
 * Запуск проверки обновлений.
 *
 * Проверка обновлений будет невозможна, если не установлена ревизия чата или исполняемый файл был переименован.
 * Ревизия автоматически устанавливается в официальных сборках собранных в http://buildbot.schat.me.
 */
void UpdatePluginImpl::check()
{
  if (m_state != Idle)
    return;

  m_state = DownloadJSON;
  m_rawJSON.clear();

  m_info = UpdateInfo(m_settings->value(m_prefix + LS("/Url")).toString() + LC('?') + QString::number(QDateTime::currentDateTime().toTime_t()));
  if (!m_info.url.isValid())
    return setDone(CheckError);

  QTimer::singleShot(0, this, SLOT(startDownload()));
}


void UpdatePluginImpl::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == m_timer->timerId())
    online();

  ChatPlugin::timerEvent(event);
}


void UpdatePluginImpl::clicked(const QString &key, QMouseEvent *event)
{
  if (m_prefix != key || event->button() != Qt::LeftButton)
    return;

  if (m_status == UpdateReady) {
    QTimer::singleShot(0, this, SLOT(restart()));
    return;
  }

  QMenu menu;

  QAction *notes    = menu.addAction(SCHAT_ICON(Globe), tr("Release Notes"));
  QAction *download = 0;
  QAction *pause    = 0;
  QAction *resume   = 0;

  if (m_state == Idle)
    download = menu.addAction(QIcon(LS(":/images/Update/download.png")), tr("Download"));

  if (m_state == DownloadUpdate)
    pause = menu.addAction(QIcon(LS(":/images/Update/pause.png")), tr("Pause"));

  if (m_state == Paused)
    resume = menu.addAction(QIcon(LS(":/images/Update/resume.png")), tr("Resume"));

  QAction *action   = menu.exec(event->globalPos());
  if (!action)
    return;

  if (action == notes) {
    QDesktopServices::openUrl(m_info.notes);
  }
  else if (action == download) {
    if (supportDownload()) {
      m_state = DownloadUpdate;
      emit done(m_status);
      QTimer::singleShot(0, this, SLOT(download()));
    }
    else
      QDesktopServices::openUrl(m_info.page);
  }
  else if (action == pause) {
    m_state = Paused;
    m_current->close();
  }
  else if (action == resume) {
    m_state = DownloadUpdate;
    emit done(m_status);
    QTimer::singleShot(0, this, SLOT(startDownload()));
  }
}


/*!
 * Запуск загрузки обновления.
 */
void UpdatePluginImpl::download()
{
  m_state = DownloadUpdate;
  m_sha1->reset();
  m_file.setFileName(Path::cache() + LS("/schat2-") + m_info.version + LS(".exe"));
  if (!m_file.open(QIODevice::WriteOnly))
    return setDone(DownloadError);

  if (BgOperationWidget::lock(m_prefix, SCHAT_UPDATE_LABEL(tr("Downloading update")))) {
    BgOperationWidget::progress()->setRange(0, m_info.size);
    BgOperationWidget::progress()->setVisible(true);
  }

  startDownload();
}


void UpdatePluginImpl::downloadProgress()
{
  if (m_state != DownloadUpdate || !BgOperationWidget::lock(m_prefix))
    return;

  BgOperationWidget::progress()->setValue(m_file.pos());
}


void UpdatePluginImpl::finished()
{
  if (!m_current->error()) {
    if (m_state == DownloadJSON)
      readJSON();
    else
      checkUpdate();
  }
  else if (m_state != Paused)
    setDone(m_state == DownloadJSON ? CheckError : DownloadError);

  m_current->deleteLater();
}


void UpdatePluginImpl::notify(const Notify &notify)
{
  if (notify.type() == Notify::PageOpen && notify.data() == ABOUT_TAB) {
    AboutTab *tab = qobject_cast<AboutTab*>(TabWidget::i()->tab(ABOUT_TAB, TabWidget::NoOptions));
    if (tab)
      tab->layout()->insertWidget(1, new UpdateWidget(this, tab));
  }
}


void UpdatePluginImpl::online()
{
  if (m_state != Idle)
    return;

  if (qAbs(DateTime::utc() - m_lastCheck) > SCHAT_UPDATE_INTERVAL)
    check();
}


void UpdatePluginImpl::readyRead()
{
  if (m_state == DownloadUpdate) {
    QByteArray data = m_current->readAll();
    m_sha1->addData(data);
    m_file.write(data);
  }
  else
    m_rawJSON.append(m_current->readAll());
}


void UpdatePluginImpl::restart()
{
  ChatNotify::start(Notify::Restart);
}


void UpdatePluginImpl::start()
{
  if (SCHAT_VER_PATH)
    QFile::remove(Path::cache() + LS("/schat2-") + QApplication::applicationVersion() + LS(".exe"));

  connect(BgOperationWidget::i(), SIGNAL(clicked(QString,QMouseEvent*)), SLOT(clicked(QString,QMouseEvent*)));
  connect(ChatClient::i(), SIGNAL(ready()), SLOT(online()));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
  check();
}


/*!
 * Загрузка файла.
 */
void UpdatePluginImpl::startDownload()
{
  QNetworkRequest request(m_info.url);
  request.setRawHeader("Referer", m_info.url.toEncoded());
  request.setRawHeader("User-Agent", QString(LS("Mozilla/5.0 (%1) Qt/%2 AppleWebKit/%3 Simple Chat/%4"))
      .arg(OsInfo::json().value(LS("os")).toString())
      .arg(qVersion())
      .arg(qWebKitVersion())
      .arg(QCoreApplication::applicationVersion()).toLatin1());

  qint64 pos = m_file.pos();
  if (pos)
    request.setRawHeader("Range", "bytes=" + QByteArray::number(pos) + "-");

  m_current = m_manager.get(request);
  connect(m_current, SIGNAL(finished()), SLOT(finished()));
  connect(m_current, SIGNAL(readyRead()), SLOT(readyRead()));
  connect(m_current, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress()));
}


/*!
 * Проверка корректности скачанного файла обновлений, методом проверки SHA1 хэша.
 */
void UpdatePluginImpl::checkUpdate()
{
  if (m_info.hash == m_sha1->result()) {
    m_settings->setValue(m_prefix + LS("/Version"),  m_info.version);
    m_settings->setValue(m_prefix + LS("/Revision"), m_info.revision);
    setDone(UpdateReady);
  }
  else
    setDone(DownloadError);
}


/*!
 * Чтение и проверка JSON данных с информацией об обновлениях.
 */
void UpdatePluginImpl::readJSON()
{
  QVariantMap data = JSON::parse(m_rawJSON).toMap();
  m_rawJSON.clear();
  if (data.isEmpty())
    return setDone(CheckError);

  QVariantMap json = data.value(m_settings->value(m_prefix + LS("/Channel")).toString()).toMap();
  if (json.isEmpty())
    return setDone(CheckError);

  m_info = UpdateInfo(json.value(LS(SCHAT_PLATFORM)).toMap());
  if (!m_info.isValid())
    return setDone(CheckError);

  m_lastCheck = DateTime::utc();

  if (int(GIT_TIMESTAMP) >= m_info.revision)
    return setDone(NoUpdates);

  setDone(UpdateAvailable);

  if (supportDownload() && m_settings->value(m_prefix + LS("/AutoDownload")).toBool() == true)
    QTimer::singleShot(0, this, SLOT(download()));
}


/*!
 * Обработка завершения операций.
 *
 * \param status Статус проверки обновлений.
 */
void UpdatePluginImpl::setDone(Status status)
{
  m_status = status;
  m_state  = Idle;

  if (m_file.isOpen())
    m_file.close();

  m_settings->setValue(m_prefix + LS("/Ready"), status == UpdateReady);

  emit done(status);

  if (!BgOperationWidget::lock(m_prefix))
    return;

  BgOperationWidget::progress()->setVisible(false);

  if (supportDownload()) {
    if (status == UpdateReady) {
      BgOperationWidget::setText(SCHAT_UPDATE_LABEL(tr("Install Update Now")));
      return;
    }

    if (status == DownloadError)
      BgOperationWidget::setText(tr("Update Error"));
  }

  if (status == UpdateAvailable) {
    BgOperationWidget::setText(SCHAT_UPDATE_LABEL(tr("Update Available")));
    return;
  }

  BgOperationWidget::unlock(m_prefix, false);
}


bool UpdatePlugin::check() const
{
  if (QString(LS(SCHAT_PLATFORM)).isEmpty())
    return false;

  if (UpdatePluginImpl::supportDownload() && Path::app() != LS("schat2"))
    return false;

  return true;
}


ChatPlugin *UpdatePlugin::create()
{
  m_plugin = new UpdatePluginImpl(this);
  return m_plugin;
}


QWidget *UpdatePlugin::settings(QWidget *parent)
{
  return new UpdateSettings(parent);
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(Update, UpdatePlugin);
#endif
