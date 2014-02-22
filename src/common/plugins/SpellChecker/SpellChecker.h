/* $Id: SpellChecker.h 2979 2012-08-07 00:59:43Z IMPOMEZIA $
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

#ifndef SPELLCHECKER_H_
#define SPELLCHECKER_H_

#include <QStringList>

#include "plugins/ChatPlugin.h"

class InputWidget;
class QAction;
class QMenu;
class QPoint;
class SettingsPage;
class SpellHighlighter;

class SpellChecker : public ChatPlugin
{
  Q_OBJECT

public:
  SpellChecker(QObject *parent = 0);
  ~SpellChecker();
  inline static SpellChecker* instance() { return m_self; }
  static QString path();
  static QString personalPath();
  static QStringList detect();
  static void reload();

private slots:
  void added(const QString &id, SettingsPage *page);
  void addToPersonalDict();
  void contextMenu(QMenu *menu, const QPoint &pos);
  void repairWord();
  void resetMenu();
  void start();
  void suggestions(const QString &word, const QStringList &words);

private:
  void suggestionsMenu(const QString &word, QMenu *parent);

  InputWidget *m_textEdit;         ///< Виджет ввода текста.
  int m_position;                  ///< Текущая позиция курсора.
  QAction *m_menuAction;           ///< Текущее действие в контекстом меню для меню вариантов слов.
  QMenu *m_menu;                   ///< Меню вариантов слов.
  QStringList m_dictionaries;      ///< Список включенных словарей.
  SpellHighlighter *m_highlighter; ///< Объект для подчёркивания неправильных слов.
  static SpellChecker *m_self;     ///< Указатель на себя.
};

#endif // SPELLCHECKER_H_
