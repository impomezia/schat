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

#ifndef SHAREWEBWIDGET_H_
#define SHAREWEBWIDGET_H_

#include <QFrame>
#include <QUrl>

class QToolButton;
class SLineEdit;

class ShareWebWidget : public QFrame
{
  Q_OBJECT

public:
  ShareWebWidget(QWidget *parent = 0);

signals:
  void upload(const QList<QUrl> &urls, bool local);

protected:
  void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private slots:
  void apply();
  void onTextChanged(const QString &text);

private:
  bool isValid(const QUrl &url) const;

  QToolButton *m_addBtn;
  SLineEdit *m_urlEdit;
};

#endif /* SHAREWEBWIDGET_H_ */
