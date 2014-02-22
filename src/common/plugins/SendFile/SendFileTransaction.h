/* $Id: SendFileTransaction.h 3056 2012-09-08 13:29:40Z IMPOMEZIA $
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

#ifndef SENDFILETRANSACTION_H_
#define SENDFILETRANSACTION_H_

#include <QSharedPointer>
#include <QString>
#include <QVariant>

namespace SendFile {

/// Определяет роль класса Transaction.
enum Role {
  SenderRole,  ///< Отправитель.
  ReceiverRole ///< Получатель.
};


/// Тип адреса.
enum AddressType {
  External,
  Internal
};


/// Состояние транзакции.
enum TransactionState {
  UnknownState      = 80, ///< 'U' Неизвестное состояние.
  WaitingState      = 87, ///< 'W' Ожидание действия пользователя.
  CancelledState    = 99, ///< 'c' Отправка файла отменена.
  ConnectingState   = 67, ///< 'C' Идёт подключение.
  TransferringState = 84, ///< 'T' Идёт передача файла.
  FinishedState     = 70  ///< 'F' Передача файла успешно завершена.
};


/*!
 * Информация о файле.
 */
struct File
{
  File()
  : size(0)
  {}

  File(const QString &name, qint64 size)
  : name(name)
  , size(size)
  {}

  QString name; ///< Имя файла.
  qint64 size;  ///< Размер файла.
};


/*!
 * Информация о внешнем и внутреннем адресе и порте.
 */
class Hosts
{
public:
  inline Hosts()
  : m_externalPort(0)
  , m_internalPort(0)
  {}

  inline Hosts(const QString &externalAddress, quint16 externalPort, const QString &internalAddress, quint16 internalPort)
  : m_externalAddress(externalAddress)
  , m_internalAddress(internalAddress)
  , m_externalPort(externalPort)
  , m_internalPort(internalPort)
  {}

  Hosts(const QVariantList &json);

  bool isValid() const;
  inline const QString& address(AddressType type = External) const { if (type) return m_internalAddress; return m_externalAddress; }
  inline quint16 port(AddressType type = External) const           { if (type) return m_internalPort; return m_externalPort; }
  QVariantList toJSON() const;

private:
  QString m_externalAddress; ///< Внешний адрес.
  QString m_internalAddress; ///< Внутренний адрес.
  quint16 m_externalPort;    ///< Внешний порт.
  quint16 m_internalPort;    ///< Внутренний порт.
};


/*!
 * Содержит информацию о передаче файла.
 */
class Transaction
{
public:
  Transaction(const QByteArray &dest, const QByteArray &id, const QString &file);
  Transaction(const QByteArray &sender, const QByteArray &id, const QVariantMap &data);
  Transaction(const QVariantMap &data);
  bool isValid() const;
  bool setLocalFile(const QString &name);

  inline bool isStarted() const                { return m_started; }
  inline bool isVisible() const                { return m_visible; }
  inline const File& file() const              { return m_file; }
  inline const Hosts& local() const            { return m_local; }
  inline const Hosts& remote() const           { return m_remote; }
  inline const QByteArray& id() const          { return m_id; }
  inline const QByteArray& user() const        { return m_user; }
  inline int percent() const                   { return m_pos * 100 / m_file.size; }
  inline qint64 pos() const                    { return m_pos; }
  inline Role role() const                     { return m_role; }
  inline TransactionState state() const        { return m_state; }

  inline void setFile(const File &file)        { m_file = file; }
  inline void setId(const QByteArray &id)      { m_id = id; }
  inline void setLocal(const Hosts &hosts)     { m_local = hosts; }
  inline void setPos(qint64 pos)               { m_pos = pos; }
  inline void setRemote(const Hosts &hosts)    { m_remote = hosts; }
  inline void setRole(Role role)               { m_role = role; }
  inline void setStarted(bool started)         { m_started = started; }
  inline void setState(TransactionState state) { m_state = state; }
  inline void setUser(const QByteArray &user)  { m_user = user; }
  inline void setVisible(bool visible = true)  { m_visible = visible; }

  QString fileName() const;
  QVariantMap toMap() const;
  QVariantMap toReceiver() const;
  void saveAs(const QString &name);

private:
  bool m_started;           ///< \b true если передача файла была запущена.
  bool m_visible;           ///< \b true если транзакция отображается в пользовательском интерфейсе, \b false если она скрыта.
  File m_file;              ///< Список файлов.
  Hosts m_local;            ///< Локальные адреса и порты.
  Hosts m_remote;           ///< Удалёные адреса и порты.
  QByteArray m_id;          ///< Идентификатор транзакции.
  QByteArray m_user;        ///< Идентификатор отправителя или получателя в зависимости от роли.
  qint64 m_pos;             ///< Текущие число отправленных или полученных байт.
  Role m_role;              ///< Роль класса.
  TransactionState m_state; ///< Состояние транзакции.
};

} // namespace SendFile

typedef QSharedPointer<SendFile::Transaction> SendFileTransaction;

#endif /* SENDFILETRANSACTION_H_ */
