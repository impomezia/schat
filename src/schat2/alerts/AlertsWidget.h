/* $Id: AlertsWidget.h 3147 2012-10-10 19:05:54Z IMPOMEZIA $
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

#ifndef ALERTSWIDGET_H_
#define ALERTSWIDGET_H_

#include <QWidget>

class QCheckBox;
class QComboBox;
class QToolBar;

class AlertsWidget : public QWidget
{
  Q_OBJECT

public:
  AlertsWidget(QWidget *parent = 0);

protected:
  void changeEvent(QEvent *event);

private slots:
  void add();
  void indexChanged(int index);
  void play();
  void popupClicked(bool checked);
  void soundChanged(const QString &file);
  void soundClicked(bool checked);
  void trayClicked(bool checked);

private:
  void retranslateUi();
  void setSoundVisible(bool visible);
  void setValue(const QString &key, const QVariant &value);

  QAction *m_add;         ///< Добавение новых звуковых файлов.
  QAction *m_control;     ///< Управление воспроизведением звукового файла.
  QCheckBox *m_popup;     ///< Флажок для включения всплывающих уведомлений.
  QCheckBox *m_sound;     ///< Флажок для включения звуковых уведомлений.
  QCheckBox *m_tray;      ///< Флажок для включения уведомлений в трее.
  QComboBox *m_combo;     ///< Комбобокс со списком уведомлений.
  QComboBox *m_file;      ///< Комбобокс для выбора звукового файла.
  QToolBar *m_toolBar;    ///< Виджет для размещения виджетов управления звуком.
};

#endif /* ALERTSWIDGET_H_ */
