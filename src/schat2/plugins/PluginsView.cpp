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

#include <QApplication>
#include <QFile>
#include <QTimer>
#include <QWebFrame>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatPlugins.h"
#include "ChatSettings.h"
#include "plugins/ChatApi.h"
#include "plugins/PluginsView.h"
#include "sglobal.h"
#include "Translation.h"
#include "WebBridge.h"

PluginsView::PluginsView(QWidget *parent)
  : WebView(parent)
{
  page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
  setFocusPolicy(Qt::NoFocus);

  connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(populateJavaScriptWindowObject()));

  QTimer::singleShot(0, this, SLOT(boot()));
}


/*!
 * Состояние плагина.
 *
 * \return - 0 - Состояние плагина не будет изменено после перезапуска.
 *         - 1 - Плагин будет включен после перезапуска чата.
 *         - 2 - Плагин будет отключен после перезапуска чата.
 */
int PluginsView::state(const QString &id)
{
  PluginItem *item = ChatCore::plugins()->plugin(id);
  if (!item)
    return 0;

  bool enabled = ChatCore::settings()->value(LS("Plugins/") + id).toBool();
  if (item->isLoaded() == enabled)
    return 0;

  if (enabled && !item->isLoaded())
    return 1;

  if (!enabled && item->isLoaded())
    return 2;

  return 0;
}


QVariantList PluginsView::list() const
{
  QVariantList plugins;

  foreach (PluginItem *item, ChatCore::plugins()->list()) {
    QVariantMap data;
    const QVariantMap &header = item->header();

    if (header.value(CORE_API_HIDDEN).toBool())
      continue;

    data[LS("id")]      = item->id();
    data[LS("icon")]    = LS("qrc") + item->icon();
    data[LS("title")]   = header.value(CORE_API_NAME);
    data[LS("version")] = header.value(CORE_API_VERSION);
    data[LS("desc")]    = desc(header);

    bool enabled = item->isLoaded();

    data[LS("enabled")]      = enabled;
    data[LS("configurable")] = enabled && header.value(CORE_API_CONFIGURABLE).toBool();

    plugins.append(data);
  }

  return plugins;
}


void PluginsView::configure(const QString &id)
{
  emit settings(id);
}


void PluginsView::enable(const QString &id, bool enable)
{
  ChatCore::settings()->setValue(LS("Plugins/") + id, enable);
}


void PluginsView::restart()
{
  QTimer::singleShot(0, this, SLOT(restartSlot()));
}


void PluginsView::scroll(const QString &id)
{
  emit scrollTo(id);
}


void PluginsView::boot()
{
  QString file = QApplication::applicationDirPath() + LS("/styles/test/html/Plugins.html");
  if (QFile::exists(file))
    file = QUrl::fromLocalFile(file).toString();
  else
    file = LS("qrc:/html/Plugins.html");

  setUrl(QUrl(file));
}


void PluginsView::populateJavaScriptWindowObject()
{
  page()->mainFrame()->addToJavaScriptWindowObject(LS("SimpleChat"), WebBridge::i());
  page()->mainFrame()->addToJavaScriptWindowObject(LS("PluginsView"), this);
}


void PluginsView::restartSlot()
{
  ChatNotify::start(Notify::Restart);
}


QString PluginsView::desc(const QVariantMap &data) const
{
  const QString &lang = ChatCore::translation()->name();
  QString desc = data.value(LS("desc/") + lang).toString();
  if (!desc.isEmpty())
    return desc;

  if (lang.size() > 2)
    desc = data.value(LS("desc/") + lang.left(2)).toString();

  if (!desc.isEmpty())
    return desc;

  return data.value(CORE_API_DESC).toString();
}
