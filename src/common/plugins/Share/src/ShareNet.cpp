/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#include <QMetaType>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QThread>

#include "ShareNet.h"
#include "uploaders/Uploader.h"
#include "Providers.h"

ShareNet::ShareNet(Providers *providers, QNetworkAccessManager *net, QObject *parent)
  : QObject(parent)
  , m_net(net)
{
  providers->create(m_uploaders, this);

  foreach (const Uploader *uploader, m_uploaders) {
    connect(uploader, SIGNAL(finished(UploadResult)), SIGNAL(finished(UploadResult)));
    connect(uploader, SIGNAL(uploadProgress(ChatId,int)), SIGNAL(uploadProgress(ChatId,int)));
  }
}


ShareNet::~ShareNet()
{
  qDeleteAll(m_uploaders);
}


void ShareNet::add(UploadItemPtr item, const QString &provider, const QVariant &data)
{
  Q_ASSERT(m_uploaders.contains(provider));

  if (!m_uploaders.contains(provider))
    return;

  m_uploaders[provider]->upload(m_net, item, data);
}


QObject *ShareNetTask::create(QNetworkAccessManager *net, QObject *parent)
{
  return new ShareNet(m_providers, net, parent);
}
