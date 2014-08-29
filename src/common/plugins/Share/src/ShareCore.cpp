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

#include <QPluginLoader>
#include <QTimer>
#include <QFileDialog>
#include <QThreadPool>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "data/ImageItem.h"
#include "interfaces/IPlugin.h"
#include "JSON.h"
#include "Providers.h"
#include "ServiceThread.h"
#include "sglobal.h"
#include "ShareButton.h"
#include "ShareChatView.h"
#include "ShareCore.h"
#include "ShareNet.h"
#include "tasks/SaveTask.h"
#include "Translation.h"
#include "ui/SendWidget.h"
#include "ui/TabWidget.h"
#include "uploaders/UploadResult.h"
#include "UploadPreview.h"

IMPORT_PLUGIN(NoneProvider)
IMPORT_PLUGIN(RupProvider)
IMPORT_PLUGIN(ImgurProvider)
IMPORT_PLUGIN(GeekpicProvider)

const QString ShareCore::kCaptureMouse = LS("Share/CaptureMouse");
const QString ShareCore::kEditor       = LS("Share/Editor");
const QString ShareCore::kLastOpenDir  = LS("Share/LastOpenDir");
const QString ShareCore::kLastSaveDir  = LS("Share/LastSaveDir");
const QString ShareCore::kProvider     = LS("Share/Provider");
const QString ShareCore::kSaveCopy     = LS("Share/SaveCopy");
const QString ShareCore::kSaveCopyIn   = LS("Share/SaveCopyIn");

ShareCore::ShareCore(QObject *parent)
  : ChatPlugin(parent)
  , m_settings(ChatCore::settings())
  , m_netId(-1)
  , m_net(0)
{
  ChatCore::translation()->addOther(LS("share"));

  qRegisterMetaType<ChatId>("ChatId");
  qRegisterMetaType<UploadItemPtr>("UploadItemPtr");
  qRegisterMetaType<Thumbnail>("Thumbnail");
  qRegisterMetaType<UploadResult>("UploadResult");

  initProviders();

  m_netId = ChatCore::service()->add(new ShareNetTask(m_providers));

  connect(ChatCore::service(), SIGNAL(ready(qint64,QObject*)), SLOT(onTaskReady(qint64,QObject*)));
}


void ShareCore::chatReady()
{
  new ShareChatView(this);

  SendWidget::add(new ShareAction(this));
}


void ShareCore::add(QRunnable *task)
{
  m_tasks.append(task);
  if (m_tasks.size() == 1)
    QTimer::singleShot(0, this, SLOT(startTasks()));
}


void ShareCore::openFile()
{
  openFile(QFileDialog::getOpenFileName(0, tr("Open image"), m_settings->value(kLastOpenDir).toString(), tr("Images (*.jpg *.jpeg *.png *.gif *.JPG *.PNG)")));
}


void ShareCore::openFile(const QString &fileName)
{
  if (fileName.isEmpty())
    return;

  QFile file(fileName);
  if (!file.open(QFile::ReadOnly) || file.size() > (10 * 1024 * 1024))
    return;

  ImageItem *image = new ImageItem();
  image->raw       = file.readAll();

  if (!image->image.loadFromData(image->raw)) {
    delete image;
    return;
  }

  if (m_settings->value(kSaveCopy).toBool())
    image->saveAs = ImageItem::toSaveAs(m_settings->value(kSaveCopyIn).toString(), QFileInfo(fileName).suffix(), image->date());

  m_settings->setValue(kLastOpenDir, QFileInfo(fileName).absolutePath());

  //TabWidget::showDialog(new UploadPreview(TabWidget::i()));
  onEditingFinished(UploadItemPtr(image));
}


void ShareCore::onEditingFinished(UploadItemPtr item)
{
  if (item->type() == ImageItem::Type) {
    ImageItem *i = uploaditem_cast<ImageItem*>(item.data());
    if (!i)
      return;

    m_pending.insert(item->id(), item);

    SaveTask *task = new SaveTask(item->id(), i->image, i->saveAs, i->raw, QSize(32, 32));
    connect(task, SIGNAL(ready(ChatId,QByteArray,Thumbnail)), SLOT(onImageSaved(ChatId,QByteArray,Thumbnail)));
    add(task);
  }
}


void ShareCore::onImageSaved(const ChatId &id, const QByteArray &body, const Thumbnail &thumbnail)
{
  if (!m_pending.contains(id))
    return;

  UploadItemPtr& item = m_pending[id];
  item->setThumbnail(thumbnail);

  if (item->type() == ImageItem::Type && ChatCore::service()->isReady()) {
    uploaditem_cast<ImageItem*>(item.data())->raw = body;

    IProvider *provider = m_providers->current();
    Q_ASSERT(provider);

    QMetaObject::invokeMethod(m_net, "add", Qt::QueuedConnection, Q_ARG(UploadItemPtr, item), Q_ARG(QString, provider->id()), Q_ARG(QVariant, provider->data()));
  }
}


void ShareCore::onTaskReady(qint64 counter, QObject *object)
{
  if (counter == m_netId) {
    m_net = qobject_cast<ShareNet*>(object);
    if (!m_net)
      return;

    connect(m_net, SIGNAL(finished(UploadResult)), SLOT(onUploadFinished(UploadResult)));
    connect(m_net, SIGNAL(uploadProgress(ChatId,int)), SLOT(onUploadProgress(ChatId,int)));
  }
}


void ShareCore::onUploadFinished(const UploadResult &result)
{
  UploadItemPtr item = m_pending.take(result.id);
  if (!item)
    return;

  qDebug() << JSON::generate(result.json, true);
}


void ShareCore::onUploadProgress(const ChatId &id, int percent)
{
  Q_UNUSED(id)
  Q_UNUSED(percent)
}


void ShareCore::startTasks()
{
  QThreadPool *pool = QThreadPool::globalInstance();

  while (!m_tasks.isEmpty())
    pool->start(m_tasks.takeFirst());
}


QObjectList ShareCore::getPlugins(const char *className) const
{
  QObjectList out;
  QObjectList instances = QPluginLoader::staticInstances();

  foreach (QObject *object, instances) {
    if (object->inherits(className))
      out.append(object);
  }

  return out;
}


void ShareCore::initProviders()
{
  m_providers = new Providers(this);
  QObjectList providers = getPlugins("IProvider");
  for (int i = 0; i < providers.size(); ++i) {
    IProvider *provider = qobject_cast<IProvider*>(providers.at(i));

    provider->init(m_settings);
    m_providers->add(provider);
  }

  m_settings->setDefault(kProvider, LS("rup"));
  m_providers->setCurrentId(m_settings->value(kProvider).toString());
}
