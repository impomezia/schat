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

#ifndef POPUPMANAGER_H_
#define POPUPMANAGER_H_

#include <QObject>

#include "schat.h"

class Alert;
class PopupWindow;
class QBasicTimer;

class PopupManager : public QObject
{
  Q_OBJECT

public:
  PopupManager(QObject *parent = 0);
  ~PopupManager();

protected:
  void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private slots:
  void popup(const Alert &alert);
  void settingsChanged(const QString &key, const QVariant &value);
  void windowDestroyed(QObject *obj);

private:
  bool isFullscreenAppActive() const;
  QString loadCSS(const QString &baseName);
  void layoutWidgets();

  bool m_flashed;                 ///< \b true если активен оранжевый стиль всплывающего окна.
  bool m_fullscreen;              ///< \b true если всплывающие окна будут показываться даже если активно полноэкранное приложение.
  bool m_stylesLoaded;            ///< \b true если CSS стили загружены.
  int m_timeout;                  ///< Время в секундах после которого всплывающие окна будут автоматически закрыты.
  QBasicTimer *m_timer;           ///< Таймер обслуживающий соединение.
  QList<PopupWindow *> m_windows; ///< Список окон.
  QString m_flashedCSS;           ///< Оранжевый QSS для всплывающего окна.
  QString m_textCSS;              ///< CSS стиль для текста всплывающего окна.
  QString m_windowsCSS;           ///< QSS для всплывающего окна.
};

#endif /* POPUPMANAGER_H_ */
