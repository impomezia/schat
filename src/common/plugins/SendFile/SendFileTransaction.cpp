/* $Id: SendFileTransaction.cpp 2817 2012-07-02 02:08:48Z IMPOMEZIA $
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

#include <QFile>
#include <QFileInfo>
#include <QHostAddress>

#include "net/SimpleID.h"
#include "SendFileTransaction.h"
#include "sglobal.h"

namespace SendFile {

Hosts::Hosts(const QVariantList &json)
  : m_externalPort(0)
  , m_internalPort(0)
{
  if (json.size() != 4)
    return;

  m_externalAddress = json.at(0).toString();
  m_externalPort    = json.at(1).toInt();
  m_internalAddress = json.at(2).toString();
  m_internalPort    = json.at(3).toInt();
}


bool Hosts::isValid() const
{
  if (!m_externalPort)
    return false;

  if (!m_internalPort)
    return false;

  if (QHostAddress(m_externalAddress).isNull())
    return false;

  if (QHostAddress(m_internalPort).isNull())
    return false;

  return true;
}


QVariantList Hosts::toJSON() const
{
  QVariantList out;
  if (!isValid())
    return out;

  out.append(m_externalAddress);
  out.append(m_externalPort);
  out.append(m_internalAddress);
  out.append(m_internalPort);
  return out;
}


Transaction::Transaction(const QByteArray &dest, const QByteArray &id, const QString &file)
  : m_started(false)
  , m_visible(false)
  , m_id(id)
  , m_user(dest)
  , m_pos(0)
  , m_role(SenderRole)
  , m_state(UnknownState)
{
  setLocalFile(file);
}


Transaction::Transaction(const QByteArray &sender, const QByteArray &id, const QVariantMap &data)
  : m_started(false)
  , m_visible(false)
  , m_remote(Hosts(data.value(LS("hosts")).toList()))
  , m_id(id)
  , m_user(sender)
  , m_pos(0)
  , m_role(ReceiverRole)
  , m_state(UnknownState)
{
  if (!m_remote.isValid())
    return;

  m_file.name = data.value(LS("name")).toString();
  m_file.size = data.value(LS("size")).toLongLong();
}


Transaction::Transaction(const QVariantMap &data)
  : m_started(false)
  , m_visible(false)
  , m_local(Hosts(data.value(LS("local")).toList()))
  , m_remote(Hosts(data.value(LS("remote")).toList()))
  , m_pos(0)
  , m_state(UnknownState)
{
  m_role   = static_cast<SendFile::Role>(data.value(LS("role")).toInt());
  m_id     = data.value(LS("id")).toByteArray();
  if (SimpleID::typeOf(m_id) != SimpleID::MessageId)
    return;

  m_user      = data.value(LS("user")).toByteArray();
  m_file.name = data.value(LS("name")).toString();
  m_file.size = data.value(LS("size")).toLongLong();
}


bool Transaction::isValid() const
{
  if (m_file.name.isEmpty())
    return false;

  return true;
}


bool Transaction::setLocalFile(const QString &name)
{
  if (!QFile::exists(name))
    return false;

  QFileInfo info(name);
  m_file.name = info.absoluteFilePath();
  m_file.size = info.size();
  return true;
}


QString Transaction::fileName() const
{
  return QFileInfo(m_file.name).fileName();
}


/*!
 * Сериализация в QVariantMap, используется для передачи в рабочий поток.
 */
QVariantMap Transaction::toMap() const
{
  QVariantMap out;
  out[LS("role")]   = m_role;
  out[LS("id")]     = m_id;
  out[LS("user")]   = m_user;
  out[LS("name")]   = m_file.name;
  out[LS("size")]   = m_file.size;
  out[LS("local")]  = m_local.toJSON();
  out[LS("remote")] = m_remote.toJSON();
  return out;
}


/*!
 * Формирование запроса с информацией о файлах для отправки получателю.
 */
QVariantMap Transaction::toReceiver() const
{
  QVariantMap json;
  if (!isValid())
    return json;

  json[LS("name")]  = fileName();
  json[LS("size")]  = m_file.size;
  json[LS("hosts")] = m_local.toJSON();

  return json;
}


/*!
 * Установка имени файла для сохранения.
 *
 * Если размер целевого файла равен 0, файл будет создан.
 */
void Transaction::saveAs(const QString &name)
{
  m_file.name = name;

  if (m_file.size == 0) {
    QFile file(name);
    file.open(QIODevice::WriteOnly);
  }
}

} // namespace SendFile
