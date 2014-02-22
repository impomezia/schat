/* $Id: ChatId.cpp 3753 2013-07-13 18:03:34Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 Alexander Sedov <support@schat.me>
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

#include <QCryptographicHash>
#include <QScopedPointer>
#include <QUuid>
#include <QNetworkInterface>

#include "base32/base32.h"
#include "id/ChatId.h"
#include "SecureRandom.h"
#include "sglobal.h"


ChatId &ChatId::init(const ObjectId &oid, qint64 modified, const uchar type)
{
  memcpy(m_oid, oid.data(), ObjectId::kOIDSize);
  m_date = modified;
  m_type = type;
  return *this;
}


/*!
 * Создание идентификатора на основе SHA1 хеша исходных данных.
 *
 * \param data Данные на основе которых будет создан идентификатор.
 * \param type Тип идентификатора.
 */
ChatId &ChatId::init(const QByteArray &data, Type type)
{
  memcpy(m_data, QCryptographicHash::hash(data, QCryptographicHash::Sha1).constData(), kIdSize - 1);
  m_data[kIdSize - 1] = type;
  return *this;
}


/*!
 * Создание идентификатора из существующих данных.
 */
ChatId &ChatId::init(const QByteArray &id)
{
  if (id.size() == kEncodedSize) {
    char outbuf[kIdSize + 1];
    if (base32_decode(reinterpret_cast<const uchar *>(id.constData()), reinterpret_cast<uchar *>(outbuf)) == kIdSize) {
      memcpy(m_data, outbuf, kIdSize);
      return *this;
    }
  }

  if (id.size() == kIdSize) {
    memcpy(m_data, id.constData(), kIdSize);
    return *this;
  }

  if (id.size() == ObjectId::kEncodedSize) {
    memcpy(m_oid, fromBase32(id).constData(), ObjectId::kOIDSize);
    m_date = 0;
    m_type = MessageId;
    return *this;
  }

  if (id.size() == ObjectId::kOIDSize) {
    memcpy(m_oid, id.constData(), ObjectId::kOIDSize);
    m_date = 0;
    m_type = MessageId;
    return *this;
  }

  clear();
  return *this;
}


/*!
 * Генерация случайного идентификатора с типом \p type и опциональной солью \p salt.
 */
ChatId &ChatId::init(Type type, const QByteArray &salt)
{
  if (type == UniqueUserId) {
#   if defined(SCHAT_RANDOM_CLIENT_ID)
    return init(QUuid::createUuid().toString().toLatin1(), type);
#   endif

    QList<QNetworkInterface> all = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface iface, all) {
      const QString hw = iface.hardwareAddress();
      const QNetworkInterface::InterfaceFlags flags = iface.flags();

      if (!hw.isEmpty() && !flags.testFlag(QNetworkInterface::IsLoopBack) && flags.testFlag(QNetworkInterface::IsUp) && flags.testFlag(QNetworkInterface::IsRunning))
        return init(hw.toLatin1() + salt, UniqueUserId);
    }

    clear();
    return *this;
  }

  const QUuid uuid  = QUuid::createUuid();
  const qint64 tail = QScopedPointer<SecureRandom>(SecureRandom::create())->nextInt64();

# if QT_VERSION >= 0x040800
  return init(salt + uuid.toRfc4122() + QByteArray::fromRawData(reinterpret_cast<const char*>(&tail), sizeof(tail)), type);
# else
  return init(salt + toRfc4122(uuid) + QByteArray::fromRawData(reinterpret_cast<const char*>(&tail), sizeof(tail)), type);
# endif
}


bool ChatId::hasOid() const
{
  if (m_type != MessageId)
    return false;

  if (m_date == 0)
    return true;

  return (oid().time() >= 946659600 && m_date >= Q_INT64_C(946659600000) && m_date < Q_INT64_C(2147483648000));
}


QByteArray ChatId::toBase32() const
{
  char outbuf[41];
  base32_encode(m_data, kIdSize, reinterpret_cast<uchar *>(outbuf));

  return QByteArray(outbuf, kEncodedSize);
}


QByteArray ChatId::fromBase32(const QByteArray &base32)
{
  const int size = UNBASE32_LEN(base32.size()) + 9;
  char *outbuf = new char[size];

  const int len = base32_decode(reinterpret_cast<const uchar *>(base32.constData()), reinterpret_cast<uchar *>(outbuf));

  const QByteArray out = QByteArray(outbuf, len);
  delete [] outbuf;
  return out;
}


QByteArray ChatId::toBase32(const QByteArray &data)
{
  const int size = BASE32_LEN(data.size()) + 1;
  char *outbuf = new char[size];

  base32_encode(reinterpret_cast<const uchar *>(data.constData()), data.size(), reinterpret_cast<uchar *>(outbuf));

  QByteArray out = QByteArray(outbuf, size - 1);
  out.replace('=', "");
  delete [] outbuf;
  return out;
}


#if QT_VERSION < 0x040800
QByteArray ChatId::toRfc4122(const QUuid uuid) const
{
  return QByteArray::fromHex(uuid.toString().remove(LC('{')).remove(LC('}')).remove(LC('-')).toLatin1());
}
#endif
