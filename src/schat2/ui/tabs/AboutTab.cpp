/* $Id: AboutTab.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QLibraryInfo>
#include <QVBoxLayout>

#include <QWebView>
#include <QApplication>
#include <QWebFrame>

# if QT_VERSION >= 0x050000
# include <QWebPage>
#else
# include <qwebkitversion.h>
#endif

#include "ChatCore.h"
#include "ChatSettings.h"
#include "Path.h"
#include "sglobal.h"
#include "Tr.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/AboutTab.h"
#include "ui/tabs/WebView.h"
#include "version.h"
#include "WebBridge.h"

class AboutTr : public Tr
{
  Q_DECLARE_TR_FUNCTIONS(AboutTr)

public:
  AboutTr() : Tr() {}

protected:
  QString valueImpl(const QString &key) const
  {
    if (key == LS("paths"))                    return tr("Paths");
    else if (key == LS("third_parties"))       return tr("Third parties");
    else if (key == LS("gnu_gpl"))             return tr("This software is released under the terms of the <a href=\"http://www.gnu.org/licenses/gpl-3.0-standalone.html\">GNU General Public License</a> version 3.");
    else if (key == LS("all_rights_reserved")) return tr("All rights reserved.");
    else if (key == LS("preferences"))         return tr("Preferences");
    return QString();
  }
};


AboutTab::AboutTab(TabWidget *parent)
  : AbstractTab(ABOUT_TAB, LS(ABOUT_TAB), parent)
{
  m_tr = new AboutTr();

  m_view = new WebView(this);
  m_view->setAcceptDrops(false);
  m_view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

  m_layout = new QVBoxLayout(this);
  m_layout->addWidget(m_view, 1);
  m_layout->setMargin(0);
  m_layout->setSpacing(0);

  connect(m_view, SIGNAL(linkClicked(QUrl)), SLOT(linkClicked(QUrl)));

  QString file = QApplication::applicationDirPath() + LS("/styles/test/html/about.html");
  if (QFile::exists(file))
    file = QUrl::fromLocalFile(file).toString();
  else
    file = LS("qrc:/html/about.html");

  m_view->setUrl(QUrl(file));
  connect(m_view->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(populateJavaScriptWindowObject()));

  setIcon(SCHAT_ICON(SmallLogo));
  retranslateUi();
}


AboutTab::~AboutTab()
{
  delete m_tr;
}


QString AboutTab::path(const QString &type) const
{
  if (type == LS("preferences"))
    return fileUrl(Path::config());

  return QString();
}


/*!
 * Получение версии из JavaScript.
 */
QVariant AboutTab::version(const QString &type) const
{
  if (type == LS("app"))
    return SCHAT_VERSION;

  else if (type == LS("Qt"))
    return qVersion() + (QSysInfo::WordSize == 32 ? tr(" (32 bit)") : tr(" (64 bit)"));

  else if (type == LS("WebKit"))
    return qWebKitVersion();

# if QT_VERSION >= 0x040700 && QT_VERSION < 0x050000
  else if (type == LS("QtWebKit"))
    return QTWEBKIT_VERSION_STR;
# endif

  return QVariant();
}


void AboutTab::linkClicked(const QUrl &url)
{
  QDesktopServices::openUrl(url);
}


void AboutTab::populateJavaScriptWindowObject()
{
  m_view->page()->mainFrame()->addToJavaScriptWindowObject(LS("SimpleChat"), WebBridge::i());
  m_view->page()->mainFrame()->addToJavaScriptWindowObject(LS("About"), this);
}


QString AboutTab::fileUrl(const QString &fileName) const
{
  QString out = LS("<a href=\"");
  out += QUrl::fromLocalFile(fileName).toEncoded();
  out += LS("\">");
  out += QDir::toNativeSeparators(fileName);
  out += LS("</a>");

  return out;
}


void AboutTab::retranslateUi()
{
  setText(tr("About"));
}


AbstractTab *AboutTabCreator::create(const QVariant &data, TabWidget *parent) const
{
  Q_UNUSED(data)
  return new AboutTab(parent);
}


void AboutTabCreator::reload(AbstractTab *tab, const QVariant &data) const
{
  Q_UNUSED(tab)
  Q_UNUSED(data);
}
