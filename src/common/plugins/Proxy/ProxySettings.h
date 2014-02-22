/* $Id: ProxySettings.h 3160 2012-10-12 21:28:03Z IMPOMEZIA $
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

#ifndef PROXYSETTINGS_H_
#define PROXYSETTINGS_H_

#include <QWidget>

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;

class ProxySettings : public QWidget
{
  Q_OBJECT

public:
  ProxySettings(QWidget *parent = 0);
  static void setProxy();

protected:
  void changeEvent(QEvent *event);

private slots:
  void addressChanged(const QString &text);
  void apply();
  void enable(bool enable);
  void reload(const QString &type);
  void save();

private:
  void load();
  void retranslateUi();
  void setVisibleAll(bool visible);

  QCheckBox *m_enable;       ///< Настройка "Proxy/Type".
  QComboBox *m_typeBox;      ///< Тип прокси сервера.
  QLabel *m_addressLabel;    ///< Адрес прокси сервера.
  QLabel *m_nameLabel;       ///< Опциональное имя для авторизации на прокси сервере.
  QLabel *m_passwordLabel;   ///< Опциональный пароль для авторизации на прокси сервере.
  QLabel *m_portLabel;       ///< Порт прокси сервера.
  QLabel *m_typeLabel;       ///< Тип прокси сервера.
  QLineEdit *m_addressEdit;  ///< Адрес прокси сервера.
  QLineEdit *m_nameEdit;     ///< Опциональное имя для авторизации на прокси сервере.
  QLineEdit *m_passwordEdit; ///< Опциональный пароль для авторизации на прокси сервере.
  QPushButton *m_apply;      ///< Кнопка применения настроек.
  QSpinBox *m_portBox;       ///< Порт прокси сервера.
};

#endif /* PROXYSETTINGS_H_ */
