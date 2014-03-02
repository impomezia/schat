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

#ifndef CHATCLIENT_H_
#define CHATCLIENT_H_

#include <QObject>
#include <QUrl>

#include "Channel.h"

class ClientChannels;
class ClientFeeds;
class ClientMessages;
class SimpleClient;

namespace Hooks
{
  class Client;
}

class SCHAT_EXPORT ChatClient : public QObject
{
  Q_OBJECT

public:
  /// Состояние клиента.
  enum ClientState {
    Online,      ///< Клиент успешно подключен
    Offline,     ///< Клиент отключен.
    Connecting,  ///< Клиент в состоянии подключения к серверу.
    Error,       ///< Критическая ошибка.
    WaitAuth     ///< Ожидание действий пользователя, он должен выбрать метод авторизации.
  };

  ChatClient(QObject *parent = 0);
  ~ChatClient();
  inline Hooks::Client *hooks() const      { return m_hooks; }
  inline static ChatClient *i()            { return m_self; }
  inline static ClientChannels *channels() { return m_self->m_channels; }
  inline static ClientFeeds *feeds()       { return m_self->m_feeds; }
  inline static ClientMessages *messages() { return m_self->m_messages; }
  static ClientChannel channel();
  static ClientChannel server();
  static int state();
  static QByteArray id();
  static QByteArray serverId();
  static QDataStream *stream();
  static qint64 date();
  static QString serverName();
  static SimpleClient *io();

  inline static bool open()                     { return open(QByteArray()); }
  inline static bool open(const QString &url)   { return open(QUrl(url)); }
  static bool open(const QByteArray &id);
  static bool open(const QUrl &url);

signals:
  void offline();
  void online();
  void ready();

private slots:
  void clientStateChanged(int state, int previousState);
  void restore();
  void setup();

private:
  ClientChannels *m_channels; ///< Каналы.
  ClientFeeds *m_feeds;       ///< Обработчик фидов.
  ClientMessages *m_messages; ///< Обработчик сообщений.
  Hooks::Client *m_hooks;     ///< Хуки.
  QByteArray m_id;            ///< Предыдущий идентификатор сервера.
  SimpleClient *m_client;     ///< Клиент чата.
  static ChatClient *m_self;  ///< Указатель на себя.
};


class SCHAT_EXPORT ChatClientLocker
{
public:
  ChatClientLocker();
  ChatClientLocker(SimpleClient *client);
  ~ChatClientLocker();
  void unlock();

private:
  SimpleClient *m_client;
};

#endif /* CHATCLIENT_H_ */
