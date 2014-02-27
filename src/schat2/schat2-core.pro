# Simple Chat
# Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

include(../common/config.pri)

SCHAT_RESOURCES = 1

TEMPLATE = lib
QT = core gui network webkit
TARGET = schat
DEFINES += SCHAT_CORE_LIBRARY
win32:RC_FILE = schat2-core.rc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia webkitwidgets

HEADERS = \
    actions/MenuBuilder.h \
    alerts/AlertsWidget.h \
    alerts/AlertType.h \
    alerts/MessageAlert.h \
    arora/lineedit.h \
    arora/lineedit_p.h \
    AuthBridge.h \
    ChatAlerts.h \
    ChatCore.h \
    ChatHooks.h \
    ChatNotify.h \
    ChatPlugins.h \
    ChatSettings.h \
    ChatUrls.h \
    hooks/ChannelMenu.h \
    hooks/ChannelMenuImpl.h \
    hooks/ChannelsImpl.h \
    hooks/ChatViewHooks.h \
    hooks/ClientFeedsImpl.h \
    hooks/ClientImpl.h \
    hooks/CommandsImpl.h \
    hooks/FeedHook.h \
    hooks/MessagesHook.h \
    hooks/MessagesImpl.h \
    hooks/PluginsPage.h \
    hooks/SendButton.h \
    hooks/ServerMenuImpl.h \
    hooks/SettingsTabImpl.h \
    hooks/ToolBarActions.h \
    hooks/UserMenuImpl.h \
    interfaces/IChatViewHook.h \
    messages/AlertMessage.h \
    messages/ChannelMessage.h \
    messages/Message.h \
    messages/ServiceMessage.h \
    NetworkManager.h \
    plugins/ChatPlugin.h \
    plugins/PluginsView.h \
    Profile.h \
    QProgressIndicator/QProgressIndicator.h \
    schat.h \
    text/FixUrlFilter.h \
    text/LinksFilter.h \
    text/TokenFilter.h \
    text/UrlFilter.h \
    ui/AlertsPixmap.h \
    ui/AuthIcon.h \
    ui/BgOperationWidget.h \
    ui/ChatIcons.h \
    ui/ColorButton.h \
    ui/fields/GenderField.h \
    ui/fields/LanguageField.h \
    ui/fields/NickEdit.h \
    ui/InputWidget.h \
    ui/LanguageBox.h \
    ui/LoginIcon.h \
    ui/MainToolBar.h \
    ui/network/NetworkButton.h \
    ui/network/NetworkComboBox.h \
    ui/network/NetworkWidget.h \
    ui/profile/ProfileField.h \
    ui/profile/ProfileFieldFactory.h \
    ui/profile/ProfileLayout.h \
    ui/profile/TextField.h \
    ui/SendWidget.h \
    ui/SLineEdit.h \
    ui/SoundButton.h \
    ui/Spinner.h \
    ui/StatusBar.h \
    ui/StatusMenu.h \
    ui/StatusWidget.h \
    ui/TabBar.h \
    ui/TabCreator.h \
    ui/tabs/AboutTab.h \
    ui/tabs/AbstractTab.h \
    ui/tabs/ChannelBaseTab.h \
    ui/tabs/ChannelTab.h \
    ui/tabs/ChatView.h \
    ui/tabs/FindWidget.h \
    ui/tabs/PrivateTab.h \
    ui/tabs/ProgressTab.h \
    ui/tabs/ServerTab.h \
    ui/tabs/SettingsTab.h \
    ui/tabs/SettingsTabHook.h \
    ui/tabs/UserView.h \
    ui/tabs/WebView.h \
    ui/tabs/WelcomeTab.h \
    ui/TabsToolBar.h \
    ui/TabWidget.h \
    ui/ToolBarAction.h \
    ui/TrayIcon.h \
    WebBridge.h \

SOURCES = \
    actions/MenuBuilder.cpp \
    alerts/AlertsWidget.cpp \
    alerts/AlertType.cpp \
    alerts/MessageAlert.cpp \
    arora/lineedit.cpp \
    AuthBridge.cpp \
    ChatAlerts.cpp \
    ChatCore.cpp \
    ChatHooks.cpp \
    ChatNotify.cpp \
    ChatPlugins.cpp \
    ChatSettings.cpp \
    ChatUrls.cpp \
    hooks/ChannelMenu.cpp \
    hooks/ChannelMenuImpl.cpp \
    hooks/ChannelsImpl.cpp \
    hooks/ChatViewHooks.cpp \
    hooks/ClientFeedsImpl.cpp \
    hooks/ClientImpl.cpp \
    hooks/CommandsImpl.cpp \
    hooks/MessagesHook.cpp \
    hooks/MessagesImpl.cpp \
    hooks/PluginsPage.cpp \
    hooks/SendButton.cpp \
    hooks/ServerMenuImpl.cpp \
    hooks/SettingsTabImpl.cpp \
    hooks/ToolBarActions.cpp \
    hooks/UserMenuImpl.cpp \
    messages/AlertMessage.cpp \
    messages/ChannelMessage.cpp \
    messages/Message.cpp \
    messages/ServiceMessage.cpp \
    NetworkManager.cpp \
    plugins/ChatPlugin.cpp \
    plugins/PluginsView.cpp \
    Profile.cpp \
    QProgressIndicator/QProgressIndicator.cpp \
    text/FixUrlFilter.cpp \
    text/LinksFilter.cpp \
    text/TokenFilter.cpp \
    text/UrlFilter.cpp \
    ui/AlertsPixmap.cpp \
    ui/AuthIcon.cpp \
    ui/BgOperationWidget.cpp \
    ui/ChatIcons.cpp \
    ui/ColorButton.cpp \
    ui/fields/GenderField.cpp \
    ui/fields/LanguageField.cpp \
    ui/fields/NickEdit.cpp \
    ui/InputWidget.cpp \
    ui/LanguageBox.cpp \
    ui/LoginIcon.cpp \
    ui/MainToolBar.cpp \
    ui/network/NetworkButton.cpp \
    ui/network/NetworkComboBox.cpp \
    ui/network/NetworkWidget.cpp \
    ui/profile/ProfileField.cpp \
    ui/profile/ProfileFieldFactory.cpp \
    ui/profile/ProfileLayout.cpp \
    ui/profile/TextField.cpp \
    ui/SendWidget.cpp \
    ui/SLineEdit.cpp \
    ui/SoundButton.cpp \
    ui/Spinner.cpp \
    ui/StatusBar.cpp \
    ui/StatusMenu.cpp \
    ui/StatusWidget.cpp \
    ui/TabBar.cpp \
    ui/tabs/AboutTab.cpp \
    ui/tabs/AbstractTab.cpp \
    ui/tabs/ChannelBaseTab.cpp \
    ui/tabs/ChannelTab.cpp \
    ui/tabs/ChatView.cpp \
    ui/tabs/FindWidget.cpp \
    ui/tabs/PrivateTab.cpp \
    ui/tabs/ProgressTab.cpp \
    ui/tabs/ServerTab.cpp \
    ui/tabs/SettingsTab.cpp \
    ui/tabs/SettingsTabHook.cpp \
    ui/tabs/UserView.cpp \
    ui/tabs/WebView.cpp \
    ui/tabs/WelcomeTab.cpp \
    ui/TabsToolBar.cpp \
    ui/TabWidget.cpp \
    ui/ToolBarAction.cpp \
    ui/TrayIcon.cpp \
    WebBridge.cpp \

SCHAT_CLIENT_LIB = 1

contains( SCHAT_EXTENSIONS, 1 ) {
  HEADERS += \
    Extension.h \
    Extensions.h \
    qzip/qzipreader.h \
    qzip/qzipwriter.h \

  SOURCES += \
    Extension.cpp \
    Extensions.cpp \
    qzip/qzip.cpp \

  contains(QT_CONFIG, system-zlib) {
    if(unix|win32-g++*):LIBS += -lz
    else:               LIBS += -lzlib
  }
  else {
    greaterThan(QT_MAJOR_VERSION, 4): DEFINES += Z_PREFIX
    INCLUDEPATH += ../3rdparty/zlib
  }
}
else {
  DEFINES += SCHAT_NO_EXTENSIONS
}

TRANSLATIONS += ../../res/translations/schat2_en.ts
TRANSLATIONS += ../../res/translations/schat2_ru.ts
TRANSLATIONS += ../../res/translations/schat2_uk.ts
CODECFORTR  = UTF-8
CODECFORSRC = UTF-8

unix {
  macx {
    target.path += ../../out/SimpleChat2.app/Contents/Frameworks/
  } else {
    include(../common/prefix.pri)

    target.path += $${LIBDIR}
  }

  INSTALLS += target
}

win32 {
  target.path += ../../os/win32/schat2/
  INSTALLS += target
  LIBS += -lshell32 -luser32
}

include(../common/common.pri)
