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

#ifndef IDOWNLOADITEM_H_
#define IDOWNLOADITEM_H_

#include <QSharedPointer>
#include <QUrl>

class INetworkError;

class IDownloadItem
{
public:
  virtual ~IDownloadItem() {}
  virtual bool isFinished() const             = 0;
  virtual const QByteArray &data() const      = 0;
  virtual INetworkError *error() const        = 0;
  virtual QByteArray &data()                  = 0;
  virtual qint64 endDate() const              = 0;
  virtual qint64 pos() const                  = 0;
  virtual qint64 size() const                 = 0;
  virtual qint64 startDate() const            = 0;
  virtual QString fileName() const            = 0;
  virtual QUrl url() const                    = 0;
  virtual void setEndDate(qint64 date)        = 0;
  virtual void setError(INetworkError *error) = 0;
  virtual void setStartDate(qint64 date)      = 0;
};

typedef QSharedPointer<IDownloadItem> DownloadItem;

#endif // IDOWNLOADITEM_H_
