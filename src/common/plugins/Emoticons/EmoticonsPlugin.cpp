/* $Id: EmoticonsPlugin.cpp 3708 2013-06-23 23:40:16Z IMPOMEZIA $
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

#include <QtPlugin>
#include <QDesktopServices>
#include <QTimer>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "Emoticons.h"
#include "EmoticonsButton.h"
#include "EmoticonsExtension.h"
#include "EmoticonsFilter.h"
#include "EmoticonsPage.h"
#include "EmoticonsPlugin.h"
#include "EmoticonsPlugin_p.h"
#include "Extensions.h"
#include "id/ChatId.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "Translation.h"
#include "ui/SendWidget.h"

EmoticonsPluginImpl::EmoticonsPluginImpl(QObject *parent)
  : ChatPlugin(parent)
{
  m_emoticons = new Emoticons(this);

  ChatCore::settings()->setDefault(LS("Emoticons"), QStringList(LS("kolobok")));
  ChatCore::extensions()->addFactory(new EmoticonsExtensionFactory());

  TokenFilter::add(LS("channel"), new EmoticonsFilter(m_emoticons));
  TokenFilter::add(LS("input"), new EmoticonsInputFilter());
  QDesktopServices::setUrlHandler(LS("emoticon"), this, "openUrl");

//  SettingsTabHook::add(new EmoticonsPageCreator(m_emoticons));

  connect(ChatCore::extensions(), SIGNAL(loaded()), SLOT(loaded()));
  connect(ChatCore::extensions(), SIGNAL(installed(QString)), SLOT(installed(QString)));

  ChatCore::translation()->addOther(LS("emoticons"));

  QTimer::singleShot(0, this, SLOT(start()));
}


void EmoticonsPluginImpl::installed(const QString &key)
{
  if (!key.startsWith(LS("emoticons/")))
    return;

  m_emoticons->load(ChatCore::extensions()->get(key));
}


void EmoticonsPluginImpl::loaded()
{
  QStringList emoticons = ChatCore::settings()->value(LS("Emoticons")).toStringList();
  foreach (const QString &name, emoticons) {
    ChatCore::extensions()->install(LS("emoticons/") + name);
  }
}


void EmoticonsPluginImpl::openUrl(const QUrl &url)
{
  ChatNotify::start(Notify::InsertText, QChar(QChar::Nbsp) + QString(ChatId::fromBase32(url.path().toLatin1())) + QChar(QChar::Nbsp));
}


void EmoticonsPluginImpl::start()
{
  SendWidget::add(new EmoticonsAction(m_emoticons));
}


bool EmoticonsPlugin::check() const
{
  return ChatCore::config().contains(LS("EXTENSIONS"));
}


ChatPlugin *EmoticonsPlugin::create()
{
  m_plugin = new EmoticonsPluginImpl(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(Emoticons, EmoticonsPlugin);
#endif
