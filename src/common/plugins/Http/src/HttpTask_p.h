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

#ifndef HTTPTASK_P_H_
#define HTTPTASK_P_H_

#include <QVariant>

class QFile;
class QNetworkReply;

class HttpTaskState
{
public:
  HttpTaskState(const QUrl &url, const QString &fileName, const QVariantMap &options);
  ~HttpTaskState();
  bool read(QNetworkReply *reply);
  inline int redirects() const { return m_redirects; }
  inline QUrl url() const      { return m_url; }
  void finish(QNetworkReply *reply);
  void redirect();

private:
  int m_redirects;
  QFile *m_file;
  qint64 m_limit;
  qint64 m_size;
  QUrl m_url;
};

#endif // HTTPTASK_P_H_
