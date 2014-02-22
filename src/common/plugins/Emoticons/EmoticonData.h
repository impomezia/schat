/* $Id: EmoticonData.h 3089 2012-09-16 15:35:14Z IMPOMEZIA $
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

#ifndef EMOTICONDATA_H_
#define EMOTICONDATA_H_

#include <QSharedPointer>
#include <QStringList>
#include <QVariant>


/*!
 * Информация о смайле.
 */
class EmoticonData
{
public:
  EmoticonData(const QString &file, const QString &id, const QVariantMap &data);
  bool isValid() const;
  inline bool isHidden() const            { return m_hidden; }
  inline const QString& file() const      { return m_file; }
  inline const QString& id() const        { return m_id; }
  inline const QStringList& texts() const { return m_texts; }
  inline int height() const               { return m_height; }
  inline int width() const                { return m_width; }

private:
  bool m_hidden;       ///< \b true если смайл скрыт.
  int m_height;        ///< Высота смайла.
  int m_width;         ///< Ширина смайла.
  QString m_file;      ///< Имя файла смайла.
  QString m_id;        ///< Идентификатор набора смайлов.
  QStringList m_texts; ///< Список текстовых сокращений смайла.
};

typedef QSharedPointer<EmoticonData> Emoticon;

#endif /* EMOTICONDATA_H_ */
