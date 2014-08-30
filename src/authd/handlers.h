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

#include "handlers/ProvidersHandler.h"
#include "handlers/StateHandler.h"
#include "oauth2/facebook/FacebookAuth.h"
#include "oauth2/github/GithubAuth.h"
#include "oauth2/google/GoogleAuth.h"
#include "oauth2/live/LiveAuth.h"
#include "oauth2/mail_ru/MailRuAuth.h"
#include "oauth2/odnoklassniki/OdnoklassnikiAuth.h"
#include "oauth2/vkontakte/VkontakteAuth.h"
#include "oauth2/yandex/YandexAuth.h"
#include "oauth2/stackoverflow/StackOverflowAuth.h"
#include "oauth2/simpleid/SimpleIdAuth.h"
