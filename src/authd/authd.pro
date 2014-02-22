# $Id: authd.pro 3677 2013-06-08 00:17:54Z IMPOMEZIA $
# IMPOMEZIA Simple Chat
# Copyright (c) 2008-2013 IMPOMEZIA <schat@impomezia.com>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program. If not, see <http://www.gnu.org/licenses/>.

SCHAT_RESOURCES   = 1
SCHAT_RC_FILE     = 1
SCHAT_SINGLEAPP   = 0
TARGET            = schat-authd
DEFINES          += SCHAT_DAEMON
SCHAT_DAEMON_LIB  = 1
SCHAT_TUFAO_LIB   = 1

QT = core network
TEMPLATE = app
CONFIG += console

DEPENDPATH  += ../schatd2
INCLUDEPATH += ../schatd2

unix:!macx {
  include(../common/prefix.pri)

  css.files += ../../res/www/css/bootstrap.css
  css.files += ../../res/www/css/main.css
  css.files += ../../res/www/css/result.css
  css.path = $${PREFIX}/share/schatd2/www/css

  img.files += ../../res/www/img/spinner.gif
  img.files += ../../res/www/img/spinner-big.gif
  img.files += ../../res/www/img/logo.png
  img.files += ../../res/www/img/providers.png
  img.path = $${PREFIX}/share/schatd2/www/img

  js.files += ../../res/www/js/html5.js
  js.files += ../../res/www/js/main.js
  js.files += ../../res/www/js/result.js
  js.files += ../../res/www/js/jquery.min.js
  js.path = $${PREFIX}/share/schatd2/www/js

  other.files += ../../res/www/receiver.html
  other.files += ../../res/www/favicon.ico
  other.path = $${PREFIX}/share/schatd2/www

  target.path += $${PREFIX}/sbin
  INSTALLS += target css img js other
}

win32 {
  css.files += ../../res/www/css/bootstrap.css
  css.files += ../../res/www/css/main.css
  css.files += ../../res/www/css/result.css
  css.path  = ../../os/win32/schatd2/www/css

  img.files += ../../res/www/img/spinner.gif
  img.files += ../../res/www/img/spinner-big.gif
  img.files += ../../res/www/img/logo.png
  img.files += ../../res/www/img/providers.png
  img.path = ../../os/win32/schatd2/www/img

  js.files += ../../res/www/js/html5.js
  js.files += ../../res/www/js/main.js
  js.files += ../../res/www/js/result.js
  js.files += ../../res/www/js/jquery.min.js
  js.path = ../../os/win32/schatd2/www/js

  other.files += ../../res/www/receiver.html
  other.files += ../../res/www/favicon.ico
  other.path = ../../os/win32/schatd2/www

  target.path += ../../os/win32/schatd2/
  INSTALLS += target css img js other
}

HEADERS = \
    AuthCore.h \
    AuthHandler.h \
    AuthState.h \
    HandlerCreator.h \
    HandlerRoute.h \
    handlers.h \
    handlers/ProvidersHandler.h \
    handlers/StateHandler.h \
    oauth2/facebook/FacebookAuth.h \
    oauth2/facebook/FacebookAuthData.h \
    oauth2/github/GithubAuth.h \
    oauth2/github/GithubAuthData.h \
    oauth2/google/GoogleAuth.h \
    oauth2/google/GoogleAuthData.h \
    oauth2/live/LiveAuth.h \
    oauth2/live/LiveAuthData.h \
    oauth2/mail_ru/MailRuAuth.h \
    oauth2/mail_ru/MailRuAuthData.h \
    oauth2/OAuthData.h \
    oauth2/OAuthHandler.h \
    oauth2/odnoklassniki/OdnoklassnikiAuth.h \
    oauth2/odnoklassniki/OdnoklassnikiAuthData.h \
    oauth2/stackoverflow/StackOverflowAuth.h \
    oauth2/stackoverflow/StackOverflowAuthData.h \
    oauth2/vkontakte/VkontakteAuth.h \
    oauth2/vkontakte/VkontakteAuthData.h \
    oauth2/yandex/YandexAuth.h \
    oauth2/yandex/YandexAuthData.h \

SOURCES = \
    AuthCore.cpp \
    authd.cpp \
    AuthHandler.cpp \
    AuthState.cpp \
    HandlerCreator.cpp \
    HandlerRoute.cpp \
    handlers/ProvidersHandler.cpp \
    handlers/StateHandler.cpp \
    oauth2/facebook/FacebookAuth.cpp \
    oauth2/facebook/FacebookAuthData.cpp \
    oauth2/github/GithubAuth.cpp \
    oauth2/github/GithubAuthData.cpp \
    oauth2/google/GoogleAuth.cpp \
    oauth2/google/GoogleAuthData.cpp \
    oauth2/live/LiveAuth.cpp \
    oauth2/live/LiveAuthData.cpp \
    oauth2/mail_ru/MailRuAuth.cpp \
    oauth2/mail_ru/MailRuAuthData.cpp \
    oauth2/OAuthData.cpp \
    oauth2/OAuthHandler.cpp \
    oauth2/odnoklassniki/OdnoklassnikiAuth.cpp \
    oauth2/odnoklassniki/OdnoklassnikiAuthData.cpp \
    oauth2/stackoverflow/StackOverflowAuth.cpp \
    oauth2/stackoverflow/StackOverflowAuthData.cpp \
    oauth2/vkontakte/VkontakteAuth.cpp \
    oauth2/vkontakte/VkontakteAuthData.cpp \
    oauth2/yandex/YandexAuth.cpp \
    oauth2/yandex/YandexAuthData.cpp \

include(../common/config.pri)
include(../common/common.pri)

