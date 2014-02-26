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

#ifndef WEBBRIDGE_H_
#define WEBBRIDGE_H_

#include <QObject>
#include <QVariant>

#include "Channel.h"

class ChannelInfo;
class FeedHook;
class FeedNotify;
class WebBridgeTr;

class SCHAT_CORE_EXPORT WebBridge : public QObject
{
  Q_OBJECT

public:
  WebBridge(QObject *parent = 0);
  ~WebBridge();
  inline static void add(FeedHook *hook) { if (m_self) m_self->m_feedHooks.append(hook); }
  inline static WebBridge *i()           { return m_self; }

  Q_INVOKABLE bool isOnline() const;
  Q_INVOKABLE int aclToInt(const QString &acl) const;
  Q_INVOKABLE int match(const QString &channelId, const QString &userId) const;
  Q_INVOKABLE qint64 mdate(const QString &id, const QString &name) const;
  Q_INVOKABLE QString bytesToHuman(qint64 size, bool html = true) const;
  Q_INVOKABLE QString day(const QString &day) const;
  Q_INVOKABLE QString id() const;
  Q_INVOKABLE QString randomId() const;
  Q_INVOKABLE QString serverId() const;
  Q_INVOKABLE QString serverPeer() const;
  Q_INVOKABLE QString status(const QString &id) const;
  Q_INVOKABLE QString statusText(int status) const;
  Q_INVOKABLE QString toBase32(const QString &text);
  Q_INVOKABLE QString translate(const QString &key, const QVariant &param = QVariant()) const;
  Q_INVOKABLE QStringList fields() const;
  Q_INVOKABLE QVariant channel(const QString &id) const;
  Q_INVOKABLE QVariant encryption() const;
  Q_INVOKABLE QVariant feed(const QString &id, const QString &name, int options = 0) const;
  Q_INVOKABLE QVariant feed(const QString &name, int options = 0) const;
  Q_INVOKABLE QVariant value(const QString &key) const;
  Q_INVOKABLE QVariantMap traffic() const;
  Q_INVOKABLE void get(const QString &id, const QString &name, const QVariantMap &json = QVariantMap());
  Q_INVOKABLE void pinTab(const QString &id) const;
  Q_INVOKABLE void request(const QString &id, const QString &command, const QString &name, const QVariantMap &json = QVariantMap());
  Q_INVOKABLE void setTabPage(const QString &id, int page);

  static QString toLink(const QByteArray &id, const QString &action = QLatin1String("insert"), const QString &name = QString());
  static QVariant feed(ClientChannel channel, const QString &name, int options = 0);
  static QVariantMap channel(ClientChannel channel, const QString &action = QLatin1String("insert"));
  static QVariantMap channel(const QByteArray &id, const QString &action = QLatin1String("insert"));
  static QVariantMap feed(const FeedNotify &notify);

  void retranslate();

signals:
  void offline();
  void online();
  void recolored(const QVariantMap &data);
  void renamed(const QVariantMap &data);
  void retranslated();
  void settingsChanged(const QString &key, const QVariant &value);
  void statusChanged(const QString &id, const QString &status);

private slots:
  void channel(const ChannelInfo &info);
  void quit(const QByteArray &user);

private:
  QList<FeedHook*> m_feedHooks; ///< Хуки фидов.
  static WebBridge *m_self;     ///< Указатель на себя.
  WebBridgeTr *m_tr;            ///< Класс перевода строк.
};

#endif /* WEBBRIDGE_H_ */
