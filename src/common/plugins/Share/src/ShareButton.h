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

#ifndef SHAREBUTTON_H_
#define SHAREBUTTON_H_

#include <QApplication>
#include <QToolButton>
#include <QUrl>

#include "ui/ToolBarAction.h"

class ShareCore;

class ShareButton : public QToolButton
{
  Q_OBJECT

public:
  ShareButton(ShareCore *core, QWidget *parent = 0);
  bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

protected:
  void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
  void addFromWeb();
  void menuAboutToHide();
  void menuAboutToShow();
  void upload(const QList<QUrl> &urls, bool local);

private:
  enum Mode {
    DefaultMode,
    WebMode
  };

  QString getDir(const QString &key) const;
  void retranslateUi();

  int m_mode;
  QMenu *m_menu;
  ShareCore *m_core;
};


class ShareAction : public ToolBarActionCreator
{
  Q_DECLARE_TR_FUNCTIONS(ShareAction)

public:
  ShareAction(ShareCore *core);
  bool isVisible(const QString &type, const QByteArray &id) Q_DECL_OVERRIDE;
  QIcon icon() const Q_DECL_OVERRIDE { return m_icon; }
  QString title() const Q_DECL_OVERRIDE;
  QWidget* createWidget(QWidget *parent) const Q_DECL_OVERRIDE;

private:
  QIcon m_icon;
  ShareCore *m_core;
};

#endif /* SHAREBUTTON_H_ */
