/* $Id: ToolBarActions.cpp 2714 2012-05-27 20:38:08Z IMPOMEZIA $
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

#include <QAction>

#include "hooks/ToolBarActions.h"
#include "sglobal.h"
#include "ui/ColorButton.h"
#include "ui/InputWidget.h"
#include "ui/SendWidget.h"

TextEditAction::TextEditAction(int action)
  : ToolBarActionCreator(TextEditWeight + action * 10)
  , m_inputAction(action)
{
  switch (action) {
    case InputWidget::Bold:
      m_name = LS("bold");
      break;

    case InputWidget::Italic:
      m_name = LS("italic");
      break;

    case InputWidget::Underline:
      m_name = LS("underline");
      break;

    case InputWidget::Strike:
      m_name = LS("strike");
      break;

    default:
      break;
  }

  if (action != InputWidget::Strike)
    m_flags |= AutoShow;
}


QAction* TextEditAction::createAction(QObject *parent) const
{
  Q_UNUSED(parent)

  if (m_name.isEmpty())
    return 0;

  return SendWidget::i()->input()->action(static_cast<InputWidget::Actions>(m_inputAction));
}


QIcon TextEditAction::icon() const
{
  QAction *action = createAction();
  if (action)
    return action->icon();

  return ToolBarActionCreator::icon();
}


QString TextEditAction::title() const
{
  QAction *action = createAction();
  if (action)
    return action->text();

  return ToolBarActionCreator::title();
}


ColorAction::ColorAction()
  : ToolBarActionCreator(ColorWeight, LS("color"), WidgetType)
{
}


QWidget* ColorAction::createWidget(QWidget *parent) const
{
  Q_UNUSED(parent)
  return SendWidget::i()->input()->color();
}


QIcon ColorAction::icon() const
{
  return QIcon(LS(":/images/color.png"));
}


QString ColorAction::title() const
{
  return tr("Text color");
}


StretchAction::StretchAction()
  : ToolBarActionCreator(StretchWeight, LS("stretch"), WidgetType | Permanent | AutoShow)
{
}


QWidget* StretchAction::createWidget(QWidget *parent) const
{
  QWidget *stretch = new QWidget(parent);
  stretch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  return stretch;
}
