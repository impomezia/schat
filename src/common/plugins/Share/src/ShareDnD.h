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

#ifndef SHAREDND_H_
#define SHAREDND_H_

#include <QStringList>
#include <QUrl>

class QMimeData;

class ShareDnD
{
  ShareDnD() {}

public:
  static QList<QUrl> getFiles(const QMimeData *data, qint64 maxSize, int maxImages);
  static QList<QUrl> getUrls(const QMimeData *data, int maxImages);

private:
  static bool addUrl(QList<QUrl> &urls, const QUrl &url);
};

#endif /* SHAREDND_H_ */
