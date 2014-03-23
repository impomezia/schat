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

#ifndef ABOUTTAB_H_
#define ABOUTTAB_H_

#include <QVariant>

#include "ui/TabCreator.h"
#include "ui/tabs/AbstractTab.h"

class AboutTr;
class QUrl;
class QVBoxLayout;
class WebView;

#define ABOUT_TAB "about"

class SCHAT_CORE_EXPORT AboutTab : public AbstractTab
{
  Q_OBJECT

public:
  AboutTab(TabWidget *parent);
  ~AboutTab();
  inline QVBoxLayout *layout() const { return m_layout; }

  Q_INVOKABLE QString path(const QString &type) const;
  Q_INVOKABLE QVariant version(const QString &type) const;

private slots:
  void linkClicked(const QUrl &url);
  void populateJavaScriptWindowObject();

private:
  QString fileUrl(const QString &fileName) const;
  void retranslateUi();

  AboutTr *m_tr;         ///< Класс перевода строк.
  QVBoxLayout *m_layout; ///< Основной компоновщик.
  WebView *m_view;       ///< Главный виджет.
};


class AboutTabCreator : public TabCreator
{
public:
  AboutTabCreator() : TabCreator() {}
  AbstractTab *create(const QVariant &data, TabWidget *parent) const;
  inline QByteArray name() const { return ABOUT_TAB; }
  void reload(AbstractTab *tab, const QVariant &data) const;
};

#endif /* ABOUTTAB_H_ */
