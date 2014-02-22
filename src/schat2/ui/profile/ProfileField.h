/* $Id: ProfileField.h 2533 2012-04-12 20:31:52Z IMPOMEZIA $
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

#ifndef PROFILEFIELD_H_
#define PROFILEFIELD_H_

#include <QWidget>

#include "schat.h"

class QLabel;
class Notify;

/*!
 * Базовый класс для полей профиля.
 */
class SCHAT_CORE_EXPORT ProfileField : public QWidget
{
  Q_OBJECT

public:
  ProfileField(const QString &field, QWidget *parent = 0);
  inline const QString& field() const { return m_field; }
  inline QLabel *label()              { return m_label; }
  static bool isMatch(const Notify &notify);
  virtual void retranslateUi();

public slots:
  void reload();

private slots:
  void notify(const Notify &notify);

protected:
  bool apply(const QVariant &value);
  virtual void setData(const QVariant &value);

  QLabel *m_label;

private:
  QString m_field;
};

#endif /* PROFILEFIELD_H_ */
