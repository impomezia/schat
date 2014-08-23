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

#include <QDesktopServices>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMenu>
#include <QTimer>
#include <QWidgetAction>
#include <QUrl>
#include <QDebug>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "sglobal.h"
#include "ShareButton.h"
#include "ShareWebWidget.h"
#include "ShareWidget.h"
#include "ui/TabWidget.h"

ShareButton::ShareButton(QWidget *parent)
  : QToolButton(parent)
  , m_mode(DefaultMode)
{
  m_menu = new QMenu(this);
  m_menu->installEventFilter(this);

  setAutoRaise(true);
  setPopupMode(QToolButton::InstantPopup);
  setMenu(m_menu);
  setStyleSheet(LS("QToolButton::menu-indicator {image:none}"));

  retranslateUi();

  connect(m_menu, SIGNAL(aboutToHide()), SLOT(menuAboutToHide()));
  connect(m_menu, SIGNAL(aboutToShow()), SLOT(menuAboutToShow()));
}


bool ShareButton::eventFilter(QObject *watched, QEvent *event)
{
  if (m_menu == watched && event->type() == QEvent::KeyPress) {
    if (static_cast<QKeyEvent *>(event)->modifiers() == Qt::AltModifier)
      return true;
  }

  return QToolButton::eventFilter( watched, event );
}


void ShareButton::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QToolButton::changeEvent(event);
}


/*!
 * Показ диалога выбора локальных файлов.
 */
void ShareButton::addFromDisk()
{
  const QString key       = LS("SendFile/SendDir");
  const QStringList files = QFileDialog::getOpenFileNames(TabWidget::i(), tr("Open images"), getDir(key), tr("Images (*.jpg *.jpeg *.png *.gif *.JPG *.PNG)"));
  if (files.isEmpty())
    return;

  QList<QUrl> urls;
  urls.reserve(files.size());

  foreach (const QString &name, files)
    urls.append(QUrl::fromLocalFile(name));

//  if (m_share->upload(ChatCore::currentId(), urls, true))
//    ChatCore::settings()->setValue(key, QFileInfo(files.first()).absolutePath());
}


void ShareButton::addFromWeb()
{
  m_mode = WebMode;
  QTimer::singleShot(0, this, SLOT(click()));
}


void ShareButton::menuAboutToHide()
{
  QList<QAction *> actions = m_menu->actions();
  foreach (QAction *action, actions)
    action->deleteLater();

  m_menu->clear();
}


void ShareButton::menuAboutToShow()
{
  QWidget *widget = 0;

  if (m_mode == DefaultMode) {
    widget = new ShareWidget(this);
    connect(widget, SIGNAL(addFromDisk()), SLOT(addFromDisk()));
    connect(widget, SIGNAL(addFromWeb()), SLOT(addFromWeb()));
  }
  else {
    m_mode = DefaultMode;
    widget = new ShareWebWidget(this);
  }

  connect(widget, SIGNAL(upload(QList<QUrl>,bool)), SLOT(upload(QList<QUrl>,bool)));

  QWidgetAction *action = new QWidgetAction(this);
  action->setDefaultWidget(widget);
  m_menu->addAction(action);
}


void ShareButton::upload(const QList<QUrl> &urls, bool local)
{
  Q_UNUSED(urls)
  Q_UNUSED(local)
//  m_share->upload(ChatCore::currentId(), urls, local);
}


QString ShareButton::getDir(const QString &key) const
{
  QDir dir(SCHAT_OPTION(key).toString());
  if (dir.path() == LS(".") || !dir.exists())
#   if QT_VERSION >= 0x050000
    dir.setPath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
#   else
    dir.setPath(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
#   endif

  return dir.absolutePath();
}


void ShareButton::retranslateUi()
{
  setToolTip(tr("Share images"));
}


ShareAction::ShareAction(ShareCore *core)
  : ToolBarActionCreator(1150, LS("share"), WidgetType | AutoShow | AutoDelete)
  , m_icon(LS(":/images/Share/button.png"))
  , m_core(core)
{
}


bool ShareAction::isVisible(const QString &type, const QByteArray &id)
{
  Q_UNUSED(type)

  const ChatId i(id);
  return i.type() == ChatId::ChannelId || i.type() == ChatId::UserId;
}


QString ShareAction::title() const
{
  return tr("Share images");
}


QWidget* ShareAction::createWidget(QWidget *parent) const
{
  ShareButton *button = new ShareButton(parent);
  button->setIcon(m_icon);
  return button;
}
