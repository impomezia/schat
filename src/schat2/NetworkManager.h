/* $Id: NetworkManager.h 2943 2012-07-29 20:30:18Z IMPOMEZIA $
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

#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#include <QHash>
#include <QObject>
#include <QSharedPointer>
#include <QStringList>
#include <QVariant>

#include "schat.h"

class NetworkManager;
class Notify;

/*!
 * Используется для хранения информации о подключении.
 * Поддерживает запись и чтение информации из конфигурационного файла.
 */
class SCHAT_CORE_EXPORT NetworkItem
{
public:
  NetworkItem(const QByteArray &id);
  bool isValid() const;
  inline const QByteArray& cookie() const { return m_cookie; }
  inline const QByteArray& id() const     { return m_id; }
  inline const QByteArray& userId() const { return m_userId; }
  inline const QString& name() const      { return m_name; }
  inline const QString& provider() const  { return m_provider; }
  inline const QString& url() const       { return m_url; }

  inline void setCookie(const QByteArray &cookie)  { m_cookie = cookie; }
  inline void setName(const QString &name)         { m_name = name; }
  inline void setUrl(const QString &url)           { m_url = url; }

  static NetworkItem* item(const QByteArray &id);

  void read();
  void write();

  friend class NetworkManager;

private:
  QString auth();
  void setAuth(const QString &auth);

  QByteArray m_cookie;   ///< Cookie.
  QByteArray m_id;       ///< Идентификатор сервера.
  QByteArray m_userId;   ///< Идентификатор пользователя.
  QString m_name;        ///< Имя сервера.
  QString m_provider;    ///< Имя авторизационного провайдера.
  QString m_url;         ///< Адрес сервера.
};

typedef QSharedPointer<NetworkItem> Network;


/*!
 * Менеджер подключений.
 */
class SCHAT_CORE_EXPORT NetworkManager : public QObject
{
  Q_OBJECT

public:
  NetworkManager(QObject *parent = 0);
  bool open();
  bool open(const QByteArray &id);

  bool isAutoConnect() const;
  inline const QByteArray& selected() const       { return m_selected; }
  inline const QByteArray& tmpId() const          { return m_tmpId; }
  int isSelectedActive() const;
  Network item(const QByteArray &id) const;
  QList<Network> items() const;

  QString root(const QByteArray &id) const;
  void removeItem(const QByteArray &id);
  void setSelected(const QByteArray &id);

  static QByteArray decode(const QByteArray &id);
  static QByteArray encode(const QByteArray &id, const QString &provider);

private slots:
  void clientStateChanged(int state);
  void notify(const Notify &notify);

private:
  void load();
  void write();

  /// Хранилище списка сетей.
  class Networks
  {
  public:
    Networks();
    QByteArray first();
    void read();
    void write();

    QList<QByteArray> data;  ///< Список идентификаторов сетей.
  };

  int m_invalids;            ///< Число элементов загруженных с ошибками.
  Networks m_networks;       ///< Список сетей.
  QByteArray m_selected;     ///< Текущая выбранная сеть в настройках.
  QByteArray m_tmpId;        ///< Временный идентификатор для текущей редактируемой сети.
  QHash<QByteArray, Network> m_items;
};

#endif /* NETWORKMANAGER_H_ */
