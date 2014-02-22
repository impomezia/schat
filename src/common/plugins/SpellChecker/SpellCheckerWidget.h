/* $Id: SpellCheckerWidget.h 2977 2012-08-06 03:43:06Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
 * Copyright © 2012 Alexey Ivanov <alexey.ivanes@gmail.com>
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

#ifndef SPELLCHECKERWIDGET_H_
#define SPELLCHECKERWIDGET_H_

#include <QWidget>

class QCheckBox;
class QLabel;
class QListWidget;

class SpellCheckerWidget : public QWidget
{
  Q_OBJECT

public:
  SpellCheckerWidget(QWidget *parent = 0);

protected:
  bool eventFilter(QObject *watched, QEvent *event);
  void changeEvent(QEvent *event);

private slots:
  void activeClicked(bool checked);
  void advancedClicked(bool checked);
  void reloadDictionaries();

private:
  QStringList checked() const;
  void build();
  void fill(const QStringList &dictionaries, const QStringList &checked);
  void retranslateUi();
  void updateVisibility();

  QCheckBox *m_active;   ///< Настройка "SpellChecker/Active".
  QCheckBox *m_advanced; ///< Настройка "SpellChecker/Advanced".
  QLabel *m_label;       ///< Надпись вверху.
  QListWidget *m_list;   ///< Список доступных языков.
};

#endif /* SPELLCHECKERWIDGET_H_ */
