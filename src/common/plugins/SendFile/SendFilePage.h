/* $Id: SendFilePage.h 2772 2012-06-13 03:25:48Z IMPOMEZIA $
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

#ifndef SENDFILEPAGE_H_
#define SENDFILEPAGE_H_

#include "ui/tabs/SettingsTabHook.h"

class QLabel;
class SendFilePluginImpl;
class QSpinBox;

class SendFilePage : public SettingsPage
{
  Q_OBJECT

public:
  SendFilePage(SendFilePluginImpl *plugin, QWidget *parent = 0);
  void retranslateUi();

private slots:
  void portChanged(int port);

private:
  void setWarning(bool visible);

  QLabel *m_label;              ///< Надпись вверху страницы.
  QLabel *m_portLabel;          ///< Надпись перед спинбоксом выбора порта.
  QLabel *m_portLabel2;         ///< Надпись после спинбокса выбора порта.
  QLabel *m_warningIcon;        ///< Иконка предупреждения.
  QLabel *m_warningLabel;       ///< Надпись предупреждающая о том что настройки будут применены при следующем запуске.
  QSpinBox *m_port;             ///< Порт для передачи файлов.
  SendFilePluginImpl *m_plugin; ///< Указатель на объект плагина.
};


class SendFilePageCreator : public SettingsPageCreator
{
public:
  SendFilePageCreator(SendFilePluginImpl *plugin)
  : SettingsPageCreator(5100)
  , m_plugin(plugin)
  {}

  SettingsPage* page(QWidget *parent = 0);

private:
  SendFilePluginImpl *m_plugin; ///< Указатель на объект плагина.
};

#endif /* SENDFILEPAGE_H_ */
