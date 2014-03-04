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

#ifndef PROFILEPLUGIN_P_H_
#define PROFILEPLUGIN_P_H_

#include <QVariant>
#include <QStringList>

#include "plugins/ChatPlugin.h"

class CountryTr;
class Notify;
class ProfilePluginTr;

class ProfilePluginImpl : public ChatPlugin
{
  Q_OBJECT

public:
  ProfilePluginImpl(QObject *parent);
  ~ProfilePluginImpl();
  void chatReady() Q_DECL_OVERRIDE;

  class Countries
  {
  public:
    static inline QString name(const QString &lang) { reload(); return m_cache.value(lang).toString(); }
    static inline QStringList list()                { reload(); return m_cache.keys(); }

  private:
    static void reload();

    static QString m_lang;
    static QVariantMap m_cache;
  };

private slots:
  void notify(const Notify &notify);
  void ready();

private:
  CountryTr *m_country;
  ProfilePluginTr *m_tr;
};


#endif /* PROFILEPLUGIN_P_H_ */
