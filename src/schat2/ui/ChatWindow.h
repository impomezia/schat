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

#ifndef CHATWINDOW_H_
#define CHATWINDOW_H_

#include <QMainWindow>
#include <QTime>

class AbstractTab;
class ChatSettings;
class Notify;
class QDesktopWidget;
class QVBoxLayout;
class SendWidget;
class StatusBar;
class TabWidget;

class ChatWindow : public QMainWindow
{
  Q_OBJECT

public:
  ChatWindow(QWidget *parent = 0);
  ~ChatWindow();

signals:
  void restartRequest();

public slots:
  void closeChat();
  void showChat();

protected:
  bool event(QEvent *event);
  void changeEvent(QEvent *event);
  void closeEvent(QCloseEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void moveEvent(QMoveEvent *event);
  void resizeEvent(QResizeEvent *event);
  void showEvent(QShowEvent *event);

# if defined(Q_OS_WIN32) && QT_VERSION < 0x050000
  bool winEvent(MSG *message, long *result);
# endif

private slots:
  void notify(const Notify &notify);
  void pageChanged(AbstractTab *tab);
  void settingsChanged(const QString &key, const QVariant &value);

private:
  QString geometryKey() const;
  void hideChat();
  void restoreGeometry();
  void retranslateUi();
  void saveGeometry();
  void setupAppIcon();
  void stylize();

  ChatSettings *m_settings;  ///< Настройки.
  QDesktopWidget *m_desktop; ///< Виджет для определения геометрии экрана.
  QTime m_activationChanged; ///< Время когда была изменена видимость окна.
  QVBoxLayout *m_mainLay;    ///< Основной компоновщик.
  QWidget *m_central;        ///< Центральный виджет.
  SendWidget *m_send;        ///< Виджет отправки сообщения.
  StatusBar *m_statusBar;    ///< Статус бар.
  TabWidget *m_tabs;         ///< Вкладки.
};

#endif /* CHATWINDOW_H_ */
