/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#ifndef PREVIEWWINDOWOBJECT_H_
#define PREVIEWWINDOWOBJECT_H_

#include <QObject>
#include <QVariant>

class ChatId;
class PreviewCore;

class PreviewWindowObject : public QObject
{
  Q_OBJECT

public:
  PreviewWindowObject(PreviewCore *core);
  Q_INVOKABLE QVariant findById(const QString &id) const;
  Q_INVOKABLE QVariant findByOID(const QString &id) const;
  Q_INVOKABLE void open(const QString &url) const;

signals:
  void changed(const QString &id);

private slots:
  void onChanged(const ChatId &id);

private:
  PreviewCore *m_core;
};

#endif // PREVIEWWINDOWOBJECT_H
