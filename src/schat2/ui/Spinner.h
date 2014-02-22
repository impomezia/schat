/* $Id: Spinner.h 2538 2012-04-13 02:08:45Z IMPOMEZIA $
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

#ifndef SPINNER_H_
#define SPINNER_H_

#include "QProgressIndicator/QProgressIndicator.h"

class SCHAT_CORE_EXPORT Spinner : public QProgressIndicator
{
  Q_OBJECT

public:
  Spinner(QWidget* parent = 0);

public slots:
  void start();
  void stop();
};

#endif /* SPINNER_H_ */
