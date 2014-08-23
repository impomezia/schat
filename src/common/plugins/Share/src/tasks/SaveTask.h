/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#ifndef SAVETASK_H_
#define SAVETASK_H_

#include <QObject>
#include <QRunnable>
#include <QImage>

#include "data/Thumbnail.h"
#include "id/ChatId.h"

/*!
 * Фоновая задача сохранения и/или конвертации файла изображения.
 */
class SaveTask : public QObject, public QRunnable
{
  Q_OBJECT

public:
  SaveTask(const ChatId &id, const QImage &image, const QString &fileName = QString(), const QByteArray &raw = QByteArray(), const QSize &resize = QSize());
  virtual void run() override;

signals:
  void ready(const ChatId &id, const QByteArray &body, const Thumbnail &thumbnail = Thumbnail());

private:
  ChatId m_id;        ///< Уникальный идентификатор.
  QByteArray m_body;  ///< Тело файла для сохранения, может быть пустым.
  QImage m_image;     ///< Изображение, может быть пустым.
  QSize m_size;       ///< Размер миниатюры.
  QString m_fileName; ///< Имя файла для сохранения, может быть пустым.
};

#endif /* SAVETASK_H_ */
