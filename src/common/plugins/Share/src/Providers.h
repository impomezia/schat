/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#ifndef PROVIDERS_H_
#define PROVIDERS_H_

#include <QMap>
#include <QObject>
#include <QStringList>

#include "interfaces/IProvider.h"

class IProvider;
class Uploader;

class Providers : public QObject
{
  Q_OBJECT

public:
  Providers(QObject *parent = 0);
  ~Providers();
  inline const QString& currentId() const        { return m_currentId; }
  inline const QStringList& list() const         { return m_list; }
  IProvider *current() const;
  IProvider *get(const QString &id) const;
  void add(IProvider *provider);
  void create(QMap<QString, Uploader*> &map, QObject *parent = 0);
  void setCurrentId(const QString &id);

private:
  QMap<QString, IProvider*> m_map; ///< Провайдеры.
  QString m_currentId;             ///< Идентификатор текущего провайдера.
  QStringList m_list;              ///< Список провайдеров.
};

#endif /* UPLOADPROVIDERS_H_ */
