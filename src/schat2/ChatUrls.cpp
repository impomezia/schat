/* $Id: ChatUrls.cpp 3708 2013-06-23 23:40:16Z IMPOMEZIA $
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

#include <QDesktopServices>
#include <QDir>

#if defined(Q_OS_WIN32)
# include <qt_windows.h>
#endif

#include "ChatNotify.h"
#include "ChatUrls.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/SimpleClient.h"
#include "net/SimpleID.h"
#include "QtEscape.h"
#include "sglobal.h"
#include "UrlQuery.h"

ChatUrls *ChatUrls::m_self = 0;

ChatUrls::ChatUrls(QObject *parent)
  : QObject(parent)
{
  m_self = this;

  QDesktopServices::setUrlHandler(LS("chat"), this, "openChatUrl");
  QDesktopServices::setUrlHandler(LS("schat"), this, "openSChatUrl");

# if defined(Q_OS_WIN32)
  QDesktopServices::setUrlHandler(LS("file"), this, "openFileUrl");
# endif
}


void ChatUrls::open(const QUrl &url)
{
  const QString scheme = url.scheme();

  if (scheme == LS("schat"))
    m_self->openSChatUrl(url);
  else if (scheme == LS("chat"))
    m_self->openChatUrl(url);
  else if (scheme != LS("qrc"))
    QDesktopServices::openUrl(url);
}


/*!
 * Получение канала из URL адреса.
 */
ClientChannel ChatUrls::channel(const QUrl &url)
{
  ClientChannel channel;
  if (url.scheme() != LS("chat") && url.host() != LS("channel"))
    return channel;

  QStringList path = ChatUrls::path(url);
  if (path.isEmpty())
    return channel;

  QByteArray id = SimpleID::decode(path.at(0).toLatin1());
  if (!Channel::isCompatibleId(id))
    return channel;

  channel = ChatClient::channels()->get(id);
  if (channel)
    return channel;

  channel = ClientChannel(new Channel(id, ChatId::fromBase32(QUrlQuery(url).queryItemValue(LS("name")).toLatin1())));
  channel->gender().setRaw(QUrlQuery(url).queryItemValue(LS("gender")).toInt());
  if (!channel->isValid())
    return ClientChannel();

  return channel;
}


QStringList ChatUrls::actions(const QUrl &url)
{
  if (url.scheme() != LS("chat"))
    return QStringList();

  QStringList out = path(url);

  if (url.host() == LS("channel")) {
    if (out.size() < 2)
      return QStringList();

    out.removeFirst();
    return out;
  }

  return out;
}


QStringList ChatUrls::path(const QUrl &url)
{
  QString path = url.path(); // В некоторых случаях путь возвращается без начального /.
  if (path.startsWith(LC('/')))
    path.remove(0, 1);

  if (path.isEmpty())
    return QStringList();

  return path.split(LC('/'), QString::SkipEmptyParts);
}


QUrl ChatUrls::fromLocalFile(const QString &localFile)
{
  QUrl url;
  url.setScheme(LS("file"));
  QString deslashified = QDir::fromNativeSeparators(localFile);

  if (deslashified.length() > 1 && deslashified.at(1) == LC(':') && deslashified.at(0) != LC('/'))
    url.setPath(LC('/') + deslashified);
  else
    url.setPath(deslashified);

  return url;
}


/*!
 * Преобразует канал в URL адрес.
 *
 * \param channel Указатель на канал.
 * \param action  Действие над каналом.
 */
QUrl ChatUrls::toUrl(ClientChannel channel, const QString &action)
{
  QUrl out(LS("chat://channel"));
  out.setPath(SimpleID::encode(channel->id()) + (action.isEmpty() ? QString() : "/" + action));

  QList<QPair<QString, QString> > queries;
  queries.append(QPair<QString, QString>(LS("name"),   ChatId::toBase32(channel->name().toUtf8())));
  queries.append(QPair<QString, QString>(LS("gender"), QString::number(channel->gender().raw())));

# if QT_VERSION >= 0x050000
  QUrlQuery query;
  query.setQueryItems(queries);
  out.setQuery(query);
# else
  out.setQueryItems(queries);
# endif

  return out;
}


/*!
 * Открытие ссылок со схемой \b chat.
 */
void ChatUrls::openChatUrl(const QUrl &url)
{
  if (url.host() == LS("channel")) {
    openChannelUrl(url);
  }
  else if (url.host() == LS("about")) {
    ChatNotify::start(Notify::OpenAbout);
  }
  else if (url.host() == LS("settings")) {
    ChatNotify::start(Notify::OpenSettings, url);
  }
}


/*!
 * Открытие ссылок со схемой \b schat.
 */
void ChatUrls::openSChatUrl(const QUrl &url)
{
  ChatClient::io()->openUrl(url);
}


#if defined(Q_OS_WIN32)
void ChatUrls::openFileUrl(const QUrl &url)
{
  if (!url.isValid())
    return;

  QString file = url.toLocalFile();
  if (file.isEmpty())
    file = url.toString();

  ShellExecute(0, 0, (wchar_t*)QDir::toNativeSeparators(file).utf16(), 0, 0, SW_SHOWNORMAL);
}
#endif


void ChatUrls::openChannelUrl(const QUrl &url)
{
  QStringList actions = ChatUrls::actions(url);
  if (actions.isEmpty())
    return;

  ClientChannel channel = ChatUrls::channel(url);
  if (!channel)
    return;

  QString action = actions.first();

  if (action == LS("open")) {
    ChatNotify::start(Notify::OpenChannel, channel->id());
  }
  else if (action == LS("info")) {
    ChatNotify::start(Notify::OpenInfo, channel->id());
  }
  else if (action == LS("insert")) {
    ChatNotify::start(Notify::InsertText, QChar(QChar::Nbsp) + QString(LS("<a class=\"nick color-%1\" href=\"%2\">%3</a>"))
        .arg(Gender::colorToString(channel->gender().color()))
        .arg(url.toString())
        .arg(Qt::escape(channel->name())) + QChar(QChar::Nbsp));
  }
  else if (action == LS("edit")) {
    if (actions.size() == 1)
      return;

    if (actions.at(1) == LS("topic") && channel->type() == SimpleID::ChannelId)
      ChatNotify::start(Notify::EditTopic, channel->id());
  }
}
