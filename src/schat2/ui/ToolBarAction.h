/* $Id: ToolBarAction.h 2769 2012-06-12 23:18:08Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#ifndef TOOLBARACTION_H_
#define TOOLBARACTION_H_

#include <QIcon>
#include <QSharedPointer>

#include "schat.h"

class QAction;

/*!
 * Базовый класс для действий, размещённых на панели инструментов.
 */
class SCHAT_CORE_EXPORT ToolBarActionCreator
{
public:
  /// Вес стандартных действий.
  enum StandartWeight {
    TextEditWeight = 1000,
    ColorWeight    = 2000,
    StretchWeight  = 10000,
    SendWeight     = 11000
  };

  /// Флаги определяющие возможности и поведение.
  enum Flag {
    NoFlags    = 0,  ///< Нет флагов.
    ActionType = 1,  ///< QAction.
    WidgetType = 2,  ///< QWidget.
    Permanent  = 4,  ///< Действие не может быть удалено.
    AutoShow   = 8,  ///< Действие будет по умолчанию отображено на панели инструментов.
    AutoDelete = 16  ///< Автоматически удалять созданное действие при удалении с панели инструментов.
  };

  Q_DECLARE_FLAGS(Flags, Flag)

  ToolBarActionCreator(int weight, const QString &name = QString(), Flags flags = ActionType);
  virtual ~ToolBarActionCreator() {}
  inline const QString& name() const     { return m_name; }
  inline Flags flags() const             { return m_flags; }
  inline int weight() const              { return m_weight; }
  inline QAction* action() const         { return m_action; }
  inline virtual QIcon icon() const      { return QIcon(); }
  inline virtual QString title() const   { return m_name; }
  inline void setAction(QAction *action) { m_action = action; }
  virtual bool isVisible(const QString &type, const QByteArray &id);
  virtual QAction* createAction(QObject *parent = 0) const;
  virtual QWidget* createWidget(QWidget *parent = 0) const;

protected:
  Flags m_flags;     ///< Флаги \sa Flags.
  QString m_name;    ///< Машинное имя действия.

private:
  int m_weight;      ///< Вес, используется для задания порядка.
  QAction *m_action; ///< Действие, равно 0 если действие не размещено на тулбаре.
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ToolBarActionCreator::Flags)
typedef QSharedPointer<ToolBarActionCreator> ToolBarAction;

#endif /* TOOLBARACTION_H_ */
