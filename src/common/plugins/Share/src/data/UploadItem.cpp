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

#include <QtEndian>
#include <QDebug>

#include "data/UploadItem.h"
#include "DateTime.h"
#include "sglobal.h"

UploadItem::UploadItem(int mode)
  : m_mode(mode)
  , m_date(0)
{
}


UploadItem::~UploadItem()
{
  qDebug() << "~UploadItem" << this;
}


bool UploadItem::isNull() const
{
  return (m_date == 0 || m_id.isNull());
}


QString UploadItem::toString() const
{
  int type;
  qToBigEndian(this->type(), (uchar*)&type);

  return QString(LS("%1/%2/%3")).arg(QString::fromLatin1((char*)&type, sizeof(int))).arg(QString(ChatId::toBase32(m_id.oid().byteArray()))).arg(mode());
}


void UploadItem::init()
{
  m_id.init(ObjectId::gen());
  m_date = DateTime::utc();
}
