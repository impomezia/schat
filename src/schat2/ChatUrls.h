/* $Id: ChatUrls.h 2787 2012-06-23 12:16:57Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#ifndef CHATURLS_H_
#define CHATURLS_H_

#include <QObject>
#include <QUrl>

#include "Channel.h"

class SCHAT_CORE_EXPORT ChatUrls : public QObject
{
  Q_OBJECT

public:
  ChatUrls(QObject *parent = 0);
  inline static ChatUrls *i() { return m_self; }
  static void open(const QUrl &url);

  static ClientChannel channel(const QUrl &url);
  static QStringList actions(const QUrl &url);
  static QStringList path(const QUrl &url);
  static QUrl fromLocalFile(const QString &localFile);
  static QUrl toUrl(ClientChannel channel, const QString &action = QString());

private slots:
  void openChatUrl(const QUrl &url);
  void openSChatUrl(const QUrl &url);

# if defined(Q_OS_WIN32)
  void openFileUrl(const QUrl &url);
# endif

private:
  void openChannelUrl(const QUrl &url);

  static ChatUrls *m_self; ///< Указатель на себя.
};

#endif /* CHATURLS_H_ */
