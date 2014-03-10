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
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStringList>

#if QT_VERSION >= 0x050000
# include <QWebPage>
#else
# include <qwebkitversion.h>
#endif

#include "HttpError.h"
#include "HttpTask.h"
#include "HttpTask_p.h"
#include "sglobal.h"
#include "tools/OsInfo.h"


HttpTaskState::HttpTaskState(const QUrl &url, const QString &fileName, const QVariantMap &options)
  : m_redirects(0)
  , m_file(0)
  , m_limit(0)
  , m_size(0)
  , m_url(url)
{
  m_limit = options.value(LS("limit")).toLongLong();
  m_mimes = options.value(LS("mimes")).toStringList();

  if (!fileName.isEmpty())
    m_file = new QFile(fileName);
}


HttpTaskState::~HttpTaskState()
{
  if (m_file)
    delete m_file;
}


bool HttpTaskState::read(QNetworkReply *reply)
{
  if (m_size == 0 && !m_mimes.isEmpty() && !m_mimes.contains(reply->header(QNetworkRequest::ContentTypeHeader).toString()))
    return false;

  if (m_limit) {
    if (m_size + reply->bytesAvailable() > m_limit)
      return false;

    if (m_size == 0 && reply->header(QNetworkRequest::ContentLengthHeader).toLongLong() > m_limit)
      return false;
  }

  m_size += reply->bytesAvailable();

  if (m_file && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
    if (!m_file->isOpen() && !m_file->open(QFile::WriteOnly))
      return false;

    const QByteArray data = reply->readAll();
    if (m_file->write(data) == -1)
      return false;
  }

  return true;
}


void HttpTaskState::finish(QNetworkReply *reply)
{
  Q_UNUSED(reply)
}


void HttpTaskState::redirect()
{
  m_size = 0;
  ++m_redirects;
}


HttpTask::HttpTask(QNetworkAccessManager *net, QObject *parent)
  : QObject(parent)
  , m_net(net)
{
}


HttpTask::~HttpTask()
{
  qDeleteAll(m_states);
}


void HttpTask::download(const QUrl &url, const QString &fileName, const QVariantMap &options)
{
  m_states.insert(url, new HttpTaskState(url, fileName, options));

  get(url);
}


/*!
 * Прогресс загрузки.
 *
 * \param bytesReceived
 * \param bytesTotal
 *
 * \sa QNetworkReply::downloadProgress()
 */
void HttpTask::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
  QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
  if (!reply)
    return;

  HttpTaskState *state = m_states.value(reply->url());
  if (!state)
    return;

  emit downloadProgress(state->url(), bytesReceived, bytesTotal);
}


/*!
 * Завершение загрузки.
 *
 * \sa QNetworkReply::finished()
 */
void HttpTask::onFinished()
{
  QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
  if (!reply)
    return;

  HttpTaskState *state = m_states.value(reply->url());
  if (!state)
    return;

  state->finish(reply);

  HttpError *error = HttpError::create(reply);
  if (error && (error->status() == 301 || error->status() == 302) && state->redirects() < 5) {
    const QUrl url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    state->redirect();
    m_states.insert(url, state);

    get(url);
    return;
  }

  emit finished(state->url(), error);

  m_states.remove(state->url());
  m_states.remove(reply->url());
  delete state;
  reply->deleteLater();
}


/*!
 * Чтение новой порции данных.
 *
 * \sa QIODevice::readyRead()
 */
void HttpTask::onReadyRead()
{
  QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
  if (!reply)
    return;

  HttpTaskState *state = m_states.value(reply->url());
  if (!state || !state->read(reply)) {
    reply->abort();
    return;
  }

  if (reply->bytesAvailable())
    emit readyRead(state->url(), reply->readAll());
}


QNetworkReply *HttpTask::get(const QUrl &url)
{
  QNetworkRequest request(url);
  request.setRawHeader("User-Agent", QString(LS("Mozilla/5.0 (%1) Qt/%2 AppleWebKit/%3 Simple Chat/%4"))
      .arg(OsInfo::json().value(LS("os")).toString())
      .arg(qVersion())
      .arg(qWebKitVersion())
      .arg(QCoreApplication::applicationVersion()).toLatin1());

  QNetworkReply *reply = m_net->get(request);

  connect(reply, SIGNAL(readyRead()), SLOT(onReadyRead()));
  connect(reply, SIGNAL(finished()), SLOT(onFinished()));
  connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(onDownloadProgress(qint64,qint64)));

  return reply;
}


QObject *HttpTaskCreator::create(QNetworkAccessManager *net, QObject *parent)
{
  return new HttpTask(net, parent);
}
