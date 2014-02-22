/* $Id: ChatAlerts.h 3385 2013-01-08 19:04:00Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#ifndef CHATALERTS_H_
#define CHATALERTS_H_

#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QQueue>

#include "schat.h"

class AlertType;
class ChatSettings;

/*!
 * Базовый класс для оповещений чата.
 */
class SCHAT_CORE_EXPORT Alert
{
public:
  Alert(const QString &type);
  Alert(const QString &type, const QByteArray &id, qint64 date);
  Alert(const QString &type, qint64 date);
  virtual ~Alert() {}

  inline const QByteArray id() const       { return m_id; }
  inline const QByteArray& tab() const     { return m_tab; }
  inline const QString& type() const       { return m_type; }
  inline const QVariantMap& data() const   { return m_data; }
  inline qint64 date() const               { return m_date; }

  inline QVariantMap& data()               { return m_data; }
  inline void setTab(const QByteArray &id) { m_tab = id; }
  void setTab(const QByteArray &sender, const QByteArray &dest);

protected:
  QByteArray m_id;     ///< Уникальный идентификатор оповещения.
  QByteArray m_tab;    ///< Идентификатор вкладки канала.
  qint64 m_date;       ///< Отметка времени.
  QString m_type;      ///< Тип оповещения.
  QVariantMap m_data;  ///< JSON данные.
};


class SCHAT_CORE_EXPORT ChatAlerts : public QObject
{
  Q_OBJECT

public:
  ChatAlerts(QObject *parent = 0);
  ~ChatAlerts();
  inline static AlertType* type(const Alert &alert) { return m_self->m_types.value(alert.type()); }
  inline static AlertType* type(const QString &id)  { return m_self->m_types.value(id); }
  inline static bool hasAlerts()                    { return !m_self->m_channels.isEmpty(); }
  inline static ChatAlerts *i()                     { return m_self; }
  inline static int count(const QByteArray &id)     { return m_count.value(id).size(); }
  inline static int total()                         { return m_alerts; }
  inline static QList<QByteArray>& channels()       { return m_channels; }
  inline static QStringList sounds()                { return m_self->m_soundMap.keys(); }
  static bool add(AlertType *type);
  static bool isMute();
  static bool isPopup();
  static bool start(const Alert &alert);
  static QByteArray last()                          { if (!m_channels.isEmpty()) return m_channels.first(); return QByteArray(); }
  static QList<AlertType*> types();
  static void play(const QString &file);
  static void remove(const QByteArray &channelId);
  static void remove(const QByteArray &channelId, const QByteArray &alertId);
  static void reset();

signals:
  void alert(bool alert);
  void alert(const Alert &alert);
  void countChanged(int total, int count, const QByteArray &channel);
  void popup(const Alert &alert);
  void removed(const QByteArray &alertId);

public slots:
  void loadSounds();

private slots:
  void offline();
  void online();
  void playSound();
  void settingsChanged(const QString &key, const QVariant &value);

private:
  static void add(const QByteArray &channelId, const QByteArray &alertId);

  bool m_popup;                                        ///< \b true если разрешены всплывающие окна.
  bool m_popupDnD;                                     ///< \b true если разрешены всплывающие окна при статусе не беспокоить.
  bool m_sounds;                                       ///< \b true если разрешены звуки.
  bool m_soundsDnD;                                    ///< \b true если разрешены звуки при статусе не беспокоить.
  ChatSettings *m_settings;                            ///< Настройки чата.
  QMap<QString, AlertType*> m_types;                   ///< Таблица типов уведомлений.
  QMap<QString, QString> m_soundMap;                   ///< Таблица соответствия имён звуковых файлов с их именами.
  QQueue<QString> m_soundQueue;                        ///< Очередь звуковых файлов на воспроизведение.
  static ChatAlerts *m_self;                           ///< Указатель на себя.
  static int m_alerts;                                 ///< Количество непрочитанных уведомлений.
  static QList<QByteArray> m_channels;                 ///< Сортированный список каналов для которых активно глобальное уведомление о новых сообщениях.
  static QMap<QByteArray, QList<QByteArray> > m_count; ///< Количество непрочитанных уведомлений для каждого канала.
};

#endif /* CHATALERTS_H_ */
