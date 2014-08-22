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

#ifndef UPLOADRESULT_H_
#define UPLOADRESULT_H_

#include <QUrl>
#include <QVariant>

#include "id/ChatId.h"

class QNetworkReply;

/*!
 * Информация о результате загрузки.
 */
class UploadResult
{
public:
  struct Image
  {
    inline Image() : height(0), size(0), width(0) {}

    int height;
    int size;
    int width;
    QString deletehash;
    QString id;
    QString title;
    QString type;
    QUrl link;
  };

  inline UploadResult() : error(0), status(0) {}
  UploadResult(QNetworkReply *reply);

  ChatId id;           ///< Идентификатор загрузки.
  int error;           ///< QNetworkReply::NetworkError.
  int status;          ///< HTTP код операции.
  QList<Image> images; ///< Информация об изображениях.
  QString desc;        ///< Описание изображения.
  QString provider;    ///< Провайдер.
  QVariant json;       ///< Сырой не обработанный JSON ответ сервера.
};

#endif /* UPLOADRESULT_H_ */
