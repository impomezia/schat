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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QMap>
#include <QSettings>

#include "interfaces/ISettings.h"
#include "schat.h"

/*!
 * Базовый класс настроек.
 */
class SCHAT_EXPORT Settings : public QSettings, public ISettings
{
  Q_OBJECT

public:
  Settings(const QString &fileName, QObject *parent = 0);
  QVariant defaultValue(const QString &key) const Q_DECL_OVERRIDE;
  QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const Q_DECL_OVERRIDE;
  void setDefault(const QString &key, const QVariant &value) Q_DECL_OVERRIDE;
  void setValue(const QString &key, const QVariant &value, bool notify = true) Q_DECL_OVERRIDE;

signals:
  void changed(const QString &key, const QVariant &value);

protected:
  mutable QMap<QString, QVariant> m_default; ///< Настройки по умолчанию.
};

#endif /* SETTINGS_H_ */
