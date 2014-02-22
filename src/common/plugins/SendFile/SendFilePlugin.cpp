/* $Id: SendFilePlugin.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QCoreApplication>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFileIconProvider>
#include <QHostAddress>
#include <QTimer>
#include <QtPlugin>
#include <QWebElement>
#include <QWebFrame>

#include "alerts/AlertType.h"
#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "ChatUrls.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/SimpleClient.h"
#include "DateTime.h"
#include "debugstream.h"
#include "hooks/ChatViewHooks.h"
#include "messages/Message.h"
#include "net/packets/MessageNotice.h"
#include "net/SimpleID.h"
#include "NetworkManager.h"
#include "Path.h"
#include "QtEscape.h"
#include "SendFileAction.h"
#include "SendFileChatView.h"
#include "SendFileDB.h"
#include "SendFileMessages.h"
#include "SendFilePage.h"
#include "SendFilePlugin.h"
#include "SendFilePlugin_p.h"
#include "SendFileThread.h"
#include "sglobal.h"
#include "Tr.h"
#include "Translation.h"
#include "ui/SendWidget.h"
#include "ui/tabs/ChannelBaseTab.h"
#include "ui/tabs/ChatView.h"
#include "ui/TabWidget.h"
#include "WebBridge.h"

#define SENDFILE_TRANSACTION(id) SendFileTransaction transaction = m_transactions.value(id); if (!transaction) return;

class SendFileTr : public Tr
{
  Q_DECLARE_TR_FUNCTIONS(SendFileTr)

public:
  SendFileTr() : Tr() { m_prefix = LS("file-"); }

protected:
  QString valueImpl(const QString &key) const
  {
    if (key == LS("waiting"))         return tr("Waiting");
    else if (key == LS("cancel"))     return tr("Cancel");
    else if (key == LS("cancelled"))  return tr("Cancelled");
    else if (key == LS("saveas"))     return tr("Save as");
    else if (key == LS("connecting")) return tr("Connecting...");
    else if (key == LS("sent"))       return tr("File sent");
    else if (key == LS("received"))   return tr("File received.");
    else if (key == LS("show"))       return tr("Show in folder");
    else if (key == LS("open"))       return tr("Open file");
    return QString();
  }
};


class IncomingFileAlertType : public AlertType
{
public:
  IncomingFileAlertType(int weight)
  : AlertType(LS("file"), weight)
  {
    m_defaults[ALERT_POPUP_KEY] = true;
    m_defaults[ALERT_TRAY_KEY]  = true;
    m_defaults[ALERT_SOUND_KEY] = true;
    m_defaults[ALERT_FILE_KEY]  = LS("Received.wav");

    m_icon = QIcon(LS(":/images/SendFile/attach.png"));
  }

  QString name() const
  {
    return QObject::tr("Incoming file");
  }
};


SendFilePluginImpl::SendFilePluginImpl(QObject *parent)
  : ChatPlugin(parent)
  , m_port(0)
  , m_db(0)
{
  ChatCore::settings()->setLocalDefault(LS("SendFile/Port"),    0);
  ChatCore::settings()->setLocalDefault(LS("SendFile/Dir"),     QString());
  ChatCore::settings()->setLocalDefault(LS("SendFile/SendDir"), QString());

  m_port = getPort();

  m_tr = new SendFileTr();
  new SendFileMessages(this);

  ChatCore::translation()->addOther(LS("sendfile"));
  QDesktopServices::setUrlHandler(LS("chat-sendfile"), this, "openUrl");
  SettingsTabHook::add(new SendFilePageCreator(this));
  ChatAlerts::add(new IncomingFileAlertType(400));
  new SendFileChatView(this);

  m_thread = new SendFile::Thread(m_port);
  connect(m_thread, SIGNAL(finished(QByteArray,qint64)), SLOT(finished(QByteArray)));
  connect(m_thread, SIGNAL(progress(QByteArray,qint64,qint64,int)), SLOT(progress(QByteArray,qint64,qint64,int)));
  connect(m_thread, SIGNAL(started(QByteArray,qint64)), SLOT(started(QByteArray)));

  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
  connect(ChatClient::i(), SIGNAL(online()), SLOT(openDB()));

  QTimer::singleShot(0, this, SLOT(start()));
}


SendFilePluginImpl::~SendFilePluginImpl()
{
  delete m_tr;
}


/*!
 * Отправка одиночного файла, пользователю будет показан диалог открытия файла.
 */
bool SendFilePluginImpl::sendFile(const QByteArray &dest)
{
  if (SimpleID::typeOf(dest) != SimpleID::UserId)
    return false;

  const QString fileName = QFileDialog::getOpenFileName(TabWidget::i(), tr("Open"), getDir(LS("SendFile/SendDir")), LS("*.*"));
  if (fileName.isEmpty())
    return false;

  ChatCore::settings()->setValue(LS("SendFile/SendDir"), QFileInfo(fileName).absolutePath());
  return sendFile(dest, fileName);
}


/*!
 * Отправка одиночного файла.
 *
 * \param dest Идентификатор получателя.
 * \param file Файл, который будет отправлен.
 */
bool SendFilePluginImpl::sendFile(const QByteArray &dest, const QString &file)
{
  if (SimpleID::typeOf(dest) != SimpleID::UserId)
    return false;

  SendFileTransaction transaction(new SendFile::Transaction(dest, ChatCore::randomId(), file));
  if (!transaction->isValid())
    return false;

  transaction->setLocal(localHosts());

  MessagePacket packet(new MessageNotice(ChatClient::id(), dest, LS("file"), DateTime::utc(), transaction->id()));
  packet->setCommand(packet->text());
  packet->setData(transaction->toReceiver());

  if (ChatClient::io()->send(packet, true)) {
    m_transactions[transaction->id()] = transaction;
    transaction->setVisible();
    transaction->setState(SendFile::WaitingState);

    Message message(transaction->id(), dest, LS("file"), LS("addFileMessage"));
    message.setAuthor(ChatClient::id());
    message.setDate();
    message.data()[LS("File")]      = transaction->fileName();
    message.data()[LS("Direction")] = LS("outgoing");
    message.data()[LS("WeakId")]    = true;
    TabWidget::add(message);
    return true;
  }

  return false;
}


int SendFilePluginImpl::setPort(quint16 port)
{
  if (m_thread->setPort(port)) {
    m_port = port;
    return port;
  }

  return m_port;
}


/*!
 * Чтение входящего пакета.
 */
void SendFilePluginImpl::read(const MessagePacket &packet)
{
# if defined(SCHAT_DEBUG)
  SCHAT_DEBUG_STREAM("[SendFile] SendFilePluginImpl::read()")
  qDebug() << "             command:" << packet->command();
  qDebug() << "             text:   " << packet->text();
  qDebug() << "             json:   " << packet->raw();
# endif

  if (packet->text() == LS("file"))
    incomingFile(packet);
  else if (packet->text() == LS("cancel"))
    cancel(packet);
  else if (packet->text() == LS("accept"))
    accept(packet);
}


int SendFilePluginImpl::role(const QString &id) const
{
  SendFileTransaction transaction = m_transactions.value(SimpleID::decode(id));
  if (!transaction)
    return SendFile::ReceiverRole;

  return transaction->role();
}


qint64 SendFilePluginImpl::size(const QString &id) const
{
  SendFileTransaction transaction = m_transactions.value(SimpleID::decode(id));
  if (!transaction)
    return 0;

  return transaction->file().size;
}


/*!
 * Получение иконки файла.
 *
 * Для входящих файлов, создаётся новый пустой файл, для него получается иконка и затем файл удаляется.
 */
QPixmap SendFilePluginImpl::fileIcon(const QString &id) const
{
  SendFileTransaction transaction = m_transactions.value(SimpleID::decode(id));
  if (!transaction)
    return QPixmap();

  if (transaction->file().name.startsWith(LS("//")))
    return makeFileIcon(transaction->fileName());

  QFileInfo info(transaction->file().name);
  if (info.isRelative() || !info.exists())
    return makeFileIcon(transaction->fileName());

  QFileIconProvider provider;
  return provider.icon(info).pixmap(16, 16);
}


QString SendFilePluginImpl::fileName(const QString &id) const
{
  SendFileTransaction transaction = m_transactions.value(SimpleID::decode(id));
  if (!transaction)
    return QString();

  return transaction->fileName();
}


QString SendFilePluginImpl::state(const QString &id) const
{
  SendFileTransaction transaction = m_transactions.value(SimpleID::decode(id));
  if (!transaction)
    return LS("U");

  return QChar((char)transaction->state());
}


QVariantMap SendFilePluginImpl::fileUrls(const QString &id) const
{
  SendFileTransaction transaction = m_transactions.value(SimpleID::decode(id));
  if (!transaction)
    return QVariantMap();

  QVariantMap out;
  out[LS("dir")]  = ChatUrls::fromLocalFile(QFileInfo(transaction->file().name).absolutePath()).toString();
  out[LS("file")] = ChatUrls::fromLocalFile(transaction->file().name).toString();
  return out;
}


QVariantMap SendFilePluginImpl::progressInfo(const QString &id) const
{
  SendFileTransaction transaction = m_transactions.value(SimpleID::decode(id));
  if (!transaction)
    return QVariantMap();

  QVariantMap out;
  out[LS("text")]    = tr("%1 of %2").arg(WebBridge::i()->bytesToHuman(transaction->pos()), WebBridge::i()->bytesToHuman(transaction->file().size));
  out[LS("percent")] = transaction->percent();
  return out;
}


void SendFilePluginImpl::sendFile()
{
  sendFile(ChatCore::currentId());
}


void SendFilePluginImpl::notify(const Notify &notify)
{
  if (notify.type() == Notify::ChannelTabClosed) {
    QByteArray id = notify.data().toByteArray();
    if (SimpleID::typeOf(id) != SimpleID::UserId)
      return;

    foreach (const SendFileTransaction &transaction, m_transactions) {
      if ((transaction->user() == id))
        transaction->setVisible(false);
    }
  }
}


void SendFilePluginImpl::openDB()
{
  if (!m_db)
    return;

  QByteArray id = ChatClient::serverId();
  if (!id.isEmpty())
    m_db->open(id, ChatCore::networks()->root(SimpleID::encode(id)));
}


void SendFilePluginImpl::openUrl(const QUrl &url)
{
  QStringList path = url.path().split(LC('/'));
  if (path.size() < 2)
    return;

  QString action = path.at(0);
  QByteArray id = SimpleID::decode(path.at(1));
  if (SimpleID::typeOf(id) != SimpleID::MessageId)
    return;

  if (action == LS("cancel"))
    cancel(id);
  else if (action == LS("saveas"))
    saveAs(id);
}


void SendFilePluginImpl::start()
{
  SendWidget::add(new SendFileAction(this));
}


void SendFilePluginImpl::finished(const QByteArray &id)
{
  SENDFILE_TRANSACTION(id)
  setState(transaction, SendFile::FinishedState);
}


void SendFilePluginImpl::progress(const QByteArray &id, qint64 current, qint64 total, int percent)
{
  SENDFILE_TRANSACTION(id)

  transaction->setPos(current);
  emit progress(SimpleID::encode(id), tr("%1 of %2").arg(WebBridge::i()->bytesToHuman(current), WebBridge::i()->bytesToHuman(total)), percent);
}


void SendFilePluginImpl::started(const QByteArray &id)
{
  SENDFILE_TRANSACTION(id)

  transaction->setStarted(true);
  setState(transaction, SendFile::TransferringState);
  progress(id, 0, transaction->file().size, 0);
}


/*!
 * Проверка на видимость транзакции.
 *
 * Если у транзакции установлен флаг видимости, выполняется проверка на реальное присутствие сообщения в окне чата.
 */
bool SendFilePluginImpl::isVisible(const SendFileTransaction &transaction)
{
  if (!transaction->isVisible())
    return false;

  ChannelBaseTab *tab = TabWidget::i()->channelTab(transaction->user(), false, false);
  if (!tab)
    return false;

  if (tab->chatView()->page()->mainFrame()->findFirstElement(LS("#" + SimpleID::encode(transaction->id()))).isNull())
    return false;

  return true;
}


MessagePacket SendFilePluginImpl::reply(const SendFileTransaction &transaction, const QString &text)
{
  MessagePacket packet(new MessageNotice(ChatClient::id(), transaction->user(), text, DateTime::utc(), transaction->id()));
  packet->setCommand(LS("file"));
  packet->setDirection(Notice::Internal);
  return packet;
}


QPixmap SendFilePluginImpl::makeFileIcon(const QString &fileName) const
{
  QFileIconProvider provider;
  QFile file(Path::cache() + LC('/') + fileName);
  file.open(QIODevice::WriteOnly);
  QFileInfo info(file);
  QPixmap pixmap = provider.icon(info).pixmap(16, 16);
  file.remove();
  return pixmap;
}


/*!
 * Получение папки в соответствии с ключом настройки \p key.
 */
QString SendFilePluginImpl::getDir(const QString &key)
{
  QDir dir(SCHAT_OPTION(key).toString());
  if (dir.path() == LS(".") || !dir.exists())
#   if QT_VERSION >= 0x050000
    dir.setPath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
#   else
    dir.setPath(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
#   endif

  return dir.absolutePath();
}


/*!
 * Определение порта для передачи файлов, если порт не определён в настройках, используется случайный порт в диапазоне от 49152 до 65536.
 */
quint16 SendFilePluginImpl::getPort() const
{
  QString key = LS("SendFile/Port");
  quint16 port = SCHAT_OPTION(key).toInt();

  if (!port) {
    qrand();
    port = qrand() % 16383 + 49152;
    ChatCore::settings()->setValue(key, port);
  }

  return port;
}


SendFile::Hosts SendFilePluginImpl::localHosts() const
{
  return SendFile::Hosts(ChatClient::io()->json().value(LS("host")).toString(), m_port, ChatClient::io()->localAddress().toString(), m_port);
}


void SendFilePluginImpl::accept(const SendFileTransaction &transaction)
{
  if (transaction->file().size > 0) {
    m_thread->add(transaction);
    setState(transaction, SendFile::ConnectingState);
  }
  else
    finished(transaction->id());
}


void SendFilePluginImpl::setState(const SendFileTransaction &transaction, SendFile::TransactionState state)
{
  transaction->setState(state);
  emit stateChanged(SimpleID::encode(transaction->id()));

  if (state == SendFile::FinishedState || state == SendFile::CancelledState) {
    if (!m_db) {
      m_db = new SendFileDB(this);
      openDB();
    }

    m_db->save(transaction);
    m_transactions.remove(transaction->id());
  }
}


/*
 * Обновление при необходимости идентификатора транзакции.
 * Используется для исходящих файлов, т.к. на момент отправки глобальный идентификатор сообщения ещё не известен.
 */
bool SendFilePluginImpl::upgrade(const MessagePacket &packet)
{
  if (packet->internalId().isEmpty())
    return false;

  SendFileTransaction transaction = m_transactions.value(packet->internalId());
  if (!transaction)
    return false;

  m_transactions.remove(transaction->id());

  transaction->setId(packet->id());
  m_transactions[transaction->id()] = transaction;

  Message message(packet->id(), packet->dest(), LS("file"), LS("upgradeFileMessage"));
  message.setDate(packet->date());
  message.data()[LS("InternalId")] = QString(SimpleID::encode(packet->internalId()));
  TabWidget::add(message);

  if (transaction->file().size > 0)
    m_thread->add(transaction);

  return false;
}


/*!
 * Принятие удалённой стороной входящего файла.
 * В поле \b hosts передаются адреса и порты удалённой стороны.
 */
void SendFilePluginImpl::accept(const MessagePacket &packet)
{
  SendFileTransaction transaction = m_transactions.value(packet->id());
  if (!transaction || transaction->role() != SendFile::SenderRole || transaction->isStarted())
    return;

  SendFile::Hosts hosts(packet->json().value(LS("hosts")).toList());
  if (!hosts.isValid())
    return;

  transaction->setRemote(hosts);
  accept(transaction);
}


/*!
 * Обработка отмены передачи файла вызванной удалённым клиентом.
 */
void SendFilePluginImpl::cancel(const MessagePacket &packet)
{
  SENDFILE_TRANSACTION(packet->id())

  m_thread->remove(packet->id());
  setState(transaction, SendFile::CancelledState);
}


/*!
 * Обработка нового входящего файла.
 */
void SendFilePluginImpl::incomingFile(const MessagePacket &packet)
{
  if (upgrade(packet))
    return;

  SendFileTransaction transaction = m_transactions.value(packet->id());
  // Создание новой транзакции.
  if (!transaction) {
    transaction = SendFileTransaction(new SendFile::Transaction(packet->sender(), packet->id(), packet->json()));
    if (!transaction->isValid())
      return;

    if (packet->status() == Notice::Found) {
      if (!m_db) {
        m_db = new SendFileDB(this);
        openDB();
      }

      m_db->restore(packet->id(), transaction);
      transaction->setUser(Message::detectTab(packet->sender(), packet->dest()));
    }
    else if (packet->status() == Notice::OK && !(packet->sender() == ChatClient::id() && packet->dest() != ChatClient::id()))
      transaction->setState(SendFile::WaitingState);

    transaction->setLocal(localHosts());
    m_transactions[transaction->id()] = transaction;
  }

  // Если транзакция уже отображается нет смысла показывать её ещё раз.
  if (isVisible(transaction))
    return;

  transaction->setVisible();

  Message message(packet->id(), Message::detectTab(packet->sender(), packet->dest()), LS("file"), LS("addFileMessage"));
  message.setAuthor(packet->sender());
  message.setDate(packet->date());
  message.data()[LS("File")]      = transaction->fileName();
  message.data()[LS("Size")]      = transaction->file().size;
  message.data()[LS("Direction")] = transaction->role() ? LS("incoming") : LS("outgoing");
  message.data()[LS("WeakId")]    = false;
  TabWidget::add(message);

  if (packet->status() != Notice::OK || !transaction->role())
    return;

  Alert alert = Alert(LS("file"), packet->id(), packet->date());
  alert.setTab(packet->sender(), packet->dest());

  AlertType *type = ChatAlerts::type(LS("file"));
  if (type && type->popup()) {
    QVariantMap popup;
    popup[LS("text")] = tr("Incoming file: %1").arg(LS("<b>") + Qt::escape(transaction->fileName()) + LS("</b>"));

    ClientChannel user = ChatClient::channels()->get(packet->sender());
    if (user)
      popup[LS("title")] = QString(LS("<b>%1</b>")).arg(Qt::escape(user->name()));

    alert.data()[LS("popup")] = popup;
  }

  ChatAlerts::start(alert);
}


/*!
 * Обработка локально вызванной отмены передачи файла.
 */
void SendFilePluginImpl::cancel(const QByteArray &id)
{
  SENDFILE_TRANSACTION(id)

  m_thread->remove(id);
  ChatClient::io()->send(reply(transaction, LS("cancel")), true);

  setState(transaction, SendFile::CancelledState);
}


/*!
 * Выбор места сохранения файла.
 */
void SendFilePluginImpl::saveAs(const QByteArray &id)
{
  SENDFILE_TRANSACTION(id)

  QString fileName = getDir(LS("SendFile/Dir")) + LC('/') + transaction->fileName();
  fileName = QFileDialog::getSaveFileName(TabWidget::i(), tr("Save"), fileName, LS("*.") + QFileInfo(fileName).suffix() + LS(";;*.*"));
  if (fileName.isEmpty())
    return;

  transaction->saveAs(fileName);
  accept(transaction);

  ChatCore::settings()->setValue(LS("SendFile/Dir"), QFileInfo(fileName).absolutePath());

  MessagePacket packet = reply(transaction, LS("accept"));
  QVariantMap data;
  data[LS("hosts")] = transaction->local().toJSON();
  packet->setData(data);
  ChatClient::io()->send(packet, true);
}


ChatPlugin *SendFilePlugin::create()
{
  m_plugin = new SendFilePluginImpl(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(SendFile, SendFilePlugin);
#endif
