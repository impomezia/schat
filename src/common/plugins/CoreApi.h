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

#ifndef COREINTERFACE_H_
#define COREINTERFACE_H_

#include <QDate>
#include <QLocale>
#include <QObject>
#include <QStringList>
#include <QVariant>

#include "schat.h"
#include "version.h"

#define CORE_API_AUTHOR       QLatin1String("author")
#define CORE_API_ID           QLatin1String("id")
#define CORE_API_NAME         QLatin1String("name")
#define CORE_API_VERSION      QLatin1String("version")
#define CORE_API_TYPE         QLatin1String("type")
#define CORE_API_SITE         QLatin1String("site")
#define CORE_API_DESC         QLatin1String("desc")
#define CORE_API_DESC_RU      QLatin1String("desc/ru")
#define CORE_API_DESC_UK      QLatin1String("desc/uk")
#define CORE_API_REQUIRED     QLatin1String("required")
#define CORE_API_ENABLED      QLatin1String("enabled")
#define CORE_API_CONFIGURABLE QLatin1String("configurable")

/*!
 * Базовый интерфейс для всех типов плагинов.
 */
class CoreApi
{
public:
  virtual ~CoreApi() {}

  /// Получение информации о плагине.
  virtual QVariantMap header() const
  {
    QVariantMap out;
    out[CORE_API_AUTHOR]       = "Alexander Sedov";  // Автор плагина.
    out[CORE_API_ID]           = QString();          // Машинное имя плагина.
    out[CORE_API_NAME]         = QString();          // Имя плагина.
    out[CORE_API_VERSION]      = "0.1.0";            // Версия плагина.
    out[CORE_API_TYPE]         = "chat";             // Тип приложения для которого предназначается плагин, например "chat", "bot" или "server".
    out[CORE_API_SITE]         = "https://schat.me"; // Домашняя страница плагина.
    out[CORE_API_DESC]         = QString();          // Описание плагина.
    out[CORE_API_REQUIRED]     = SCHAT_VERSION;      // Версия чата необходимая для работы плагина.
    out[CORE_API_ENABLED]      = true;               // \b true если плагин по умолчанию включен.
    out[CORE_API_CONFIGURABLE] = false;              // \b true если поддерживается дополнительный пользовательский интерфейс настроек.

    return out;
  }
};

Q_DECLARE_INTERFACE(CoreApi, "me.schat.CoreApi/1.2");

#endif /* COREINTERFACE_H_ */
