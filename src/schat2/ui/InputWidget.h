/* $Id: InputWidget.h 3766 2013-08-13 17:12:24Z IMPOMEZIA $
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

#ifndef INPUTWIDGET_H_
#define INPUTWIDGET_H_

#include <QTextEdit>

#include "id/ChatId.h"

class ColorButton;
class QToolBar;
class QWidgetAction;

class SCHAT_CORE_EXPORT InputWidget : public QTextEdit
{
  Q_OBJECT

public:
  enum Actions {
    Bold,
    Italic,
    Underline,
    Strike
  };

  InputWidget(QWidget *parent = 0);
  ColorButton *color();
  inline QStringList history() const     { return m_states[m_id].history; }
  inline QAction *action(Actions action) { return m_format.at(action); }
  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  void reload(const QByteArray &id);
  void setMsg(int index);

signals:
  void contextMenu(QMenu *menu, const QPoint &pos);
  void focusOut();
  void reloaded();
  void send(const QString &text);

protected:
  void changeEvent(QEvent *event);
  void contextMenuEvent(QContextMenuEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void insertFromMimeData(const QMimeData *source);
  void keyPressEvent(QKeyEvent *event);
  void resizeEvent(QResizeEvent *event);
  void showEvent(QShowEvent *event);

public slots:
  void clear();
  void paste();
  void send();

private slots:
  void cursorPositionChanged();
  void menuTriggered(QAction *action);
  void setBold(bool bold);
  void setItalic(bool italic);
  void setStrike(bool strike);
  void setTextColor(const QColor &color);
  void setUnderline(bool underline);
  void textChanged();

private:
  class State
  {
  public:
    inline State() : current(0), pos(0) {}

    int current;         ///< Текущее сообщение в истории.
    int pos;             ///< Текущая позиция курсора.
    QString html;        ///< Текущий html текст в виджете.
    QStringList history; ///< Отправленные сообщения.
  };

  bool bypass(QKeyEvent *event);
  int textHeight(int lines = 0) const;
  void createActions();
  void mergeFormat(const QTextCharFormat &format);
  void nextMsg();
  void prevMsg();
  void retranslateUi();

  bool m_resizable;               ///< true если виджет может автоматически изменять свои размеры.
  ChatId m_id;                    ///< Идентификатор текущего разговора.
  ColorButton *m_color;           ///< Кнопка выбора цвета.
  int m_lines;                    ///< Минимальная высота текста в строчках.
  QAction *m_clear;               ///< Clear.
  QAction *m_copy;                ///< Copy.
  QAction *m_cut;                 ///< Cut.
  QAction *m_paste;               ///< Paste.
  QAction *m_selectAll;           ///< Select All.
  QList<QAction *> m_format;      ///< Действия связанные с форматированием текста.
  QMap<ChatId, State> m_states;   ///< Индивидуальные состояния поля отправки для разных каналов.
  QMenu *m_menu;                  ///< Контекстное меню.
  QTextCharFormat m_default;      ///< Формат текста по умолчанию.
  QToolBar *m_toolBar;            ///< Панель инструментов в контекстом меню.
  QWidgetAction *m_action;        ///< Действие для добавления панели инструментов в меню.
};

#endif /* INPUTWIDGET_H_ */
