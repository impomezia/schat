/* $Id: ChHook.cpp 3441 2013-01-24 22:14:27Z IMPOMEZIA $
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

#include "Ch.h"
#include "ChHook.h"
#include "net/packets/auth.h"

ChHook::ChHook(QObject *parent)
  : QObject(parent)
{
  Ch::add(this);
}


ChHook::~ChHook()
{
  Ch::remove(this);
}


/*!
 * Добавление канала.
 */
void ChHook::add(ChatChannel channel)
{
  Q_UNUSED(channel)
}


/*!
 * Загрузка основных каналов сервера.
 */
void ChHook::load()
{
}


/*!
 * Создание нового обычного канала.
 *
 * \param channel Указатель на канал.
 * \param user    Пользователь создавший канал, если есть такой.
 */
void ChHook::newChannel(ChatChannel channel, ChatChannel user)
{
  Q_UNUSED(channel)
  Q_UNUSED(user)
}


/*!
 * Удаление канала.
 */
void ChHook::remove(ChatChannel channel)
{
  Q_UNUSED(channel)
}


/*!
 * Обработка успешного переименования канала.
 */
void ChHook::rename(ChatChannel channel)
{
  Q_UNUSED(channel);
}


/*!
 * Инициализация серверного канала.
 *
 * \param channel Канал сервера.
 * \param created \b true если канал был создан, фактически это происходит только при первом запуске сервера.
 */
void ChHook::server(ChatChannel channel, bool created)
{
  Q_UNUSED(channel)
  Q_UNUSED(created)
}


/*!
 * Синхронизация обычного канала.
 *
 * \param channel Указатель на канал.
 * \param user    Пользователь создавший канал, если есть такой.
 */
void ChHook::sync(ChatChannel channel, ChatChannel user)
{
  Q_UNUSED(channel)
  Q_UNUSED(user)
}


/*!
 * Создание нового или успешная авторизация существующего пользователя.
 */
void ChHook::userChannel(ChatChannel channel)
{
  Q_UNUSED(channel)
}


/*!
 * Создание нового или успешная авторизация существующего пользователя.
 *
 * \param channel Канал-пользователь.
 * \param data    Авторизационные данные.
 * \param host    Адрес пользователя.
 * \param created \b true если пользователь был создан.
 * \param socket  Сокет пользователя.
 */
void ChHook::userChannel(ChatChannel channel, const AuthRequest &data, const QString &host, bool created, quint64 socket)
{
  Q_UNUSED(channel)
  Q_UNUSED(data)
  Q_UNUSED(host)
  Q_UNUSED(created)
  Q_UNUSED(socket)
}
