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

#ifndef SENDFILEPLUGIN_P_H_
#define SENDFILEPLUGIN_P_H_

#include <QVariant>
#include <QPixmap>

#include "net/packets/MessageNotice.h"
#include "plugins/ChatPlugin.h"
#include "SendFileTransaction.h"

class ChatView;
class Notify;
class SendFileDB;
class SendFileTr;

namespace SendFile {
  class Thread;
}

/*!
 * Основной класс плагина SendFile.
 *
 * \todo Добавить поддержку отображения скорости передачи файла.
 * \todo Добавить возможность ставить передачу файла на паузу.
 * \todo Сделать возможным передачу файла в случае разрыва соединения.
 */
class SendFilePluginImpl : public ChatPlugin
{
  Q_OBJECT

public:
  SendFilePluginImpl(QObject *parent);
  ~SendFilePluginImpl();

  bool sendFile(const QByteArray &dest);
  bool sendFile(const QByteArray &dest, const QString &file);
  int setPort(quint16 port);
  void chatReady() Q_DECL_OVERRIDE;
  void read(const MessagePacket &packet);

  Q_INVOKABLE int role(const QString &id) const;
  Q_INVOKABLE qint64 size(const QString &id) const;
  Q_INVOKABLE QPixmap fileIcon(const QString &id) const;
  Q_INVOKABLE QString fileName(const QString &id) const;
  Q_INVOKABLE QString state(const QString &id) const;
  Q_INVOKABLE QVariantMap fileUrls(const QString &id) const;
  Q_INVOKABLE QVariantMap progressInfo(const QString &id) const;

signals:
  void progress(const QString &id, const QString &text, int percent);
  void stateChanged(const QString &id);

public slots:
  void sendFile();

private slots:
  void notify(const Notify &notify);
  void openDB();
  void openUrl(const QUrl &url);
  void start();

  void finished(const QByteArray &id);
  void progress(const QByteArray &id, qint64 current, qint64 total, int percent);
  void started(const QByteArray &id);

private:
  bool isVisible(const SendFileTransaction &transaction);
  MessagePacket reply(const SendFileTransaction &transaction, const QString &text);
  QPixmap makeFileIcon(const QString &fileName) const;
  QString getDir(const QString &key);
  quint16 getPort() const;
  SendFile::Hosts localHosts() const;
  void accept(const SendFileTransaction &transaction);
  void setState(const SendFileTransaction &transaction, SendFile::TransactionState state);

  bool upgrade(const MessagePacket &packet);
  void accept(const MessagePacket &packet);
  void cancel(const MessagePacket &packet);
  void incomingFile(const MessagePacket &packet);

  // Реакция на действия пользователя.
  void cancel(const QByteArray &id);
  void saveAs(const QByteArray &id);

  QHash<QByteArray, SendFileTransaction> m_transactions; ///< Отправленные файлы и входящие файлы.
  quint16 m_port;                                        ///< Порт который будет использоваться для передачи файлов.
  SendFile::Thread *m_thread;                            ///< Рабочий поток передачи файлов.
  SendFileDB *m_db;                                      ///< База данных для сохранения информации о передаче файлов.
  SendFileTr *m_tr;                                      ///< Класс перевода строк.
};

#endif /* SENDFILEPLUGIN_P_H_ */
