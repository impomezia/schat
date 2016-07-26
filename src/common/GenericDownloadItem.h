/* Simple Chat
 * Copyright (c) 2008-2016 Alexander Sedov <imp@schat.me>
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

#ifndef GENERICDOWNLOADITEM_H_
#define GENERICDOWNLOADITEM_H_

#include "interfaces/IDownloadItem.h"
#include "interfaces/INetworkError.h"
#include "schat.h"

class GenericDownloadItem : public IDownloadItem
{
public:
  inline GenericDownloadItem(const QUrl &url, const QString &fileName)
    : m_error(0)
    , m_endDate(0)
    , m_pos(0)
    , m_size(0)
    , m_startDate(0)
    , m_fileName(fileName)
    , m_url(url)
  {}

  inline ~GenericDownloadItem()
  {
    if (m_error)
      delete m_error;
  }

  bool isFinished() const             Q_DECL_OVERRIDE { return m_endDate > 0; }
  const QByteArray &data() const      Q_DECL_OVERRIDE { return m_data; }
  INetworkError *error() const        Q_DECL_OVERRIDE { return m_error; }
  QByteArray &data()                  Q_DECL_OVERRIDE { return m_data; }
  qint64 endDate() const              Q_DECL_OVERRIDE { return m_endDate; }
  qint64 pos() const                  Q_DECL_OVERRIDE { return m_pos; }
  qint64 size() const                 Q_DECL_OVERRIDE { return m_size; }
  qint64 startDate() const            Q_DECL_OVERRIDE { return m_startDate; }
  QString fileName() const            Q_DECL_OVERRIDE { return m_fileName; }
  QUrl url() const                    Q_DECL_OVERRIDE { return m_url; }
  void setEndDate(qint64 date)        Q_DECL_OVERRIDE { m_endDate = date; }
  void setError(INetworkError *error) Q_DECL_OVERRIDE { if (m_error) delete m_error; m_error = error; }
  void setStartDate(qint64 date)      Q_DECL_OVERRIDE { m_startDate = date; }

protected:
  INetworkError *m_error;
  QByteArray m_data;
  qint64 m_endDate;
  qint64 m_pos;
  qint64 m_size;
  qint64 m_startDate;
  QString m_fileName;
  QUrl m_url;
};

#endif // GENERICDOWNLOADITEM_H_
