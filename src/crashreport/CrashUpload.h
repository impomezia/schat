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

#ifndef CRASHUPLOAD_H_
#define CRASHUPLOAD_H_

#include <QObject>
#include <QFileInfo>

class QNetworkAccessManager;
class QNetworkReply;

class CrashUpload : public QObject
{
  Q_OBJECT

public:
  CrashUpload(const QFileInfoList &files, QObject *parent = 0);
  ~CrashUpload();

private slots:
  void onFinished(QNetworkReply *reply);
  void start();

private:
  const QFileInfoList m_files;
  int m_error;
  QNetworkAccessManager *m_net;
};

#endif // CRASHUPLOAD_H_
