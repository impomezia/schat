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

#ifndef CHATVIEW_H_
#define CHATVIEW_H_

#include <QQueue>
#include <QVariant>
#include <QWebView>

#include "id/ChatId.h"
#include "ui/tabs/WebView.h"

class FeedNotify;
class Message;
class Notify;

class SCHAT_CORE_EXPORT ChatView : public WebView
{
  Q_OBJECT

public:
  ChatView(const QByteArray &id, const QString &url, QWidget *parent = 0);
  ~ChatView();

  bool find(const QString &text, bool forward = true);
  inline const QByteArray& id() const { return m_id; }
  void add(const Message &msg);
  void addCSS(const QString &file);
  void copy();
  void evaluateJavaScript(const QString &func, const QVariant &param);
  void evaluateJavaScript(const QString &js);
  void setId(const QByteArray &id);

  Q_INVOKABLE bool openDialog(const QString &id, const QVariant &data);
  Q_INVOKABLE inline qint64 lastMessage() const      { return m_lastMessage; }
  Q_INVOKABLE inline QStringList jsfiles() const     { return m_jsfiles; }
  Q_INVOKABLE inline void addJS(const QString &file) { if (!m_jsfiles.contains(file)) m_jsfiles.append(file); }
  Q_INVOKABLE QString getId() const;
  Q_INVOKABLE void allowFocus(bool allow);
  Q_INVOKABLE void loadFinished();
  Q_INVOKABLE void removeDay(const QString &day);
  Q_INVOKABLE void setLastMessage(qint64 date);

signals:
  void feed(const QVariantMap &data);
  void messages(const QVariantList &messages);
  void reload();

protected:
  void contextMenu(QMenu *menu, const QWebHitTestResult &result);
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  void retranslateUi();
  void showEvent(QShowEvent *event);

private slots:
  void alignChat();
  void menuTriggered(QAction *action);
  void notify(const Notify &notify);
  void openUrl(const QUrl &url);
  void populateJavaScriptWindowObject();
  void settingsChanged(const QString &key, const QVariant &value);
  void start();
  void startTasks();

private:
  QVariantMap addHint(const Message &message);
  void clearPage();
  void createActions();
  void messageEditorMenu(QMenu *menu);
  void reloadPage();

  bool m_loaded;                                       ///< true если документ загружен.
  QAction *m_autoscroll;                               ///< Автопрокрутка.
  QAction *m_clear;                                    ///< Очистить.
  QAction *m_reload;                                   ///< Обновить.
  QAction *m_seconds;                                  ///< Секунды.
  QAction *m_service;                                  ///< Сервисные сообщения.
  QByteArray m_id;                                     ///< Идентификатор.
  qint64 m_lastMessage;                                ///< Время последнего сообщения.
  QMap<QString, QMap<qint64, ChatId> > m_messages;     ///< Сортированные по времени сообщения.
  QQueue<QString> m_pendingJs;                         ///< Очередь JavaScript кода ожидающего загрузки документа.
  QQueue<QVariantMap> m_pendingFeeds;                  ///< Очередь данных фидов ожидающих загрузки документа.
  QStringList m_jsfiles;                               ///< Дополнительные динамически загружаемые JavaScript скрипты.
  QVariantList m_messagesQueue;                        ///< Очередь сообщений ожидающих загрузки документа.
};

#endif /* CHATVIEW_H_ */
