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

#include "DateTime.h"
#include "id/ChatId.h"
#include "JSON.h"
#include "net/NetRequest.h"
#include "sglobal.h"

const QString NetRequest::AUTH  = QLatin1String("AUTH");
const QString NetRequest::DEL   = QLatin1String("DEL");
const QString NetRequest::GET   = QLatin1String("GET");
const QString NetRequest::POST  = QLatin1String("POST");
const QString NetRequest::PUB   = QLatin1String("PUB");
const QString NetRequest::PUT   = QLatin1String("PUT");
const QString NetRequest::SUB   = QLatin1String("SUB");
const QString NetRequest::UNSUB = QLatin1String("UNSUB");

NetRequest::NetRequest()
  : id(genId())
  , date(0)
{
}


NetRequest::NetRequest(const QVariantList &list)
{
  if (list.size() < 7)
    return;

  id      = list.at(1).toString();
  date    = list.at(2).toLongLong();
  method  = list.at(3).toString();
  request = list.at(4).toString();
  headers = list.at(5).toMap();
  data    = list.mid(6);
}


bool NetRequest::isValid() const
{
  if (id.isEmpty() || method.isEmpty())
    return false;

  return true;
}


QByteArray NetRequest::toJSON() const
{
  QVariantList list;
  list.append(LS("REQ"));
  list.append(id);
  list.append(date);
  list.append(method);
  list.append(request);
  list.append(headers);

  if (data.canConvert(QVariant::List)) {
    const QVariantList d = data.toList();
    if (!d.isEmpty())
      list.append(data.toList());
    else
      list.append(QVariant());
  }
  else
    list.append(data);

  return JSON::generate(list);
}


QString NetRequest::genId()
{
  return ChatId::toBase32(ObjectId::gen().byteArray());
}
