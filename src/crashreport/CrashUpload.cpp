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
#include <QDir>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

#include "CrashUpload.h"
#include "sglobal.h"

CrashUpload::CrashUpload(const QFileInfoList &files, QObject *parent)
  : QObject(parent)
  , m_files(files)
  , m_error(-1)
{
  m_net = new QNetworkAccessManager(this);
  connect(m_net, SIGNAL(finished(QNetworkReply*)), SLOT(onFinished(QNetworkReply*)));

  QTimer::singleShot(0, this, SLOT(start()));
}


CrashUpload::~CrashUpload()
{
  if (m_error != 0)
    return;

  foreach (const QFileInfo &info, m_files) {
    QFile::remove(info.absoluteFilePath());
  }
}


void CrashUpload::onFinished(QNetworkReply *reply)
{
  m_error = reply->error();

  reply->deleteLater();

  QCoreApplication::exit(m_error);
}


void CrashUpload::start()
{
  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  foreach (const QFileInfo &info, m_files) {
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"minidump\"; filename=\"" + info.fileName().toUtf8() + "\"");

    QFile *file = new QFile(info.absoluteFilePath(), multiPart);
    if (file->open(QFile::ReadOnly)) {
      part.setBodyDevice(file);
      multiPart->append(part);
    }
  }

  QHttpPart path;
  path.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"path\""));
  path.setBody(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()).toUtf8());
  multiPart->append(path);

  QNetworkRequest request(QUrl(LS("https://crashdumps.schat.me/1/upload")));

  QNetworkReply *reply = m_net->post(request, multiPart);
  multiPart->setParent(reply);
}
