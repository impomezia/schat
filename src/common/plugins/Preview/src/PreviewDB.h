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

#ifndef PREVIEWDB_H_
#define PREVIEWDB_H_

#include <QObject>
#include <QUrl>

#include "id/ChatId.h"

struct ImageRecord
{
  inline ImageRecord() : width(0), height(0), size(0) {}

  ChatId id;
  QUrl url;
  QString format;
  int width;
  int height;
  int size;
  QString name;
};


class PreviewDB : public QObject
{
  Q_OBJECT

public:
  PreviewDB(QObject *parent = 0);
  bool open(const QString &path);
  ImageRecord *findById(const ChatId &id);
  void save(const ChatId &id, const QUrl &url);

private:
  void create();
  void version();

  const QString m_id;
};

#endif // PREVIEWDB_H_
