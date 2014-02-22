/* $Id: UpdateWidget.h 3380 2013-01-07 01:34:20Z IMPOMEZIA $
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

#ifndef UPDATEWIDGET_H_
#define UPDATEWIDGET_H_

#include <QFrame>

class QLabel;
class Spinner;
class UpdatePluginImpl;

class UpdateWidget : public QFrame
{
  Q_OBJECT

public:
  UpdateWidget(UpdatePluginImpl *plugin, QWidget *parent = 0);

private slots:
  void reload();

private:
  QLabel *m_icon;             ///< Иконка.
  QLabel *m_text;             ///< Основной текст.
  Spinner *m_spinner;         ///< Спиннер.
  UpdatePluginImpl *m_plugin; ///< Указатель на плагин.
};

#endif /* UPDATEWIDGET_H_ */
