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

#ifndef POPUPSETTINGS_H_
#define POPUPSETTINGS_H_

#include <QWidget>

#include "schat.h"

class QCheckBox;
class QLabel;
class QSpinBox;

class PopupSettings : public QWidget
{
  Q_OBJECT

public:
  PopupSettings(QWidget *parent = 0);

protected:
  void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
  void enable(bool enable);
  void settingsChanged(const QString &key, const QVariant &value);
  void timeChanged(int time);

private:
  void retranslateUi();

  QCheckBox *m_enable; ///< Настройка "Alerts/Popup".
  QLabel *m_timeHintLabel;
  QLabel *m_timeLabel; ///< Надпись поясняющая действие опции "Alerts/PopupTimeout".
  QSpinBox *m_timeBox; ///< Настройка "Alerts/PopupTimeout".
};

#endif /* POPUPSETTINGS_H_ */
