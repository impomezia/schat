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

#ifndef IPLUGIN_H_
#define IPLUGIN_H_

#include <QtPlugin>

class IPlugin
{
public:
  virtual ~IPlugin() {}
};

Q_DECLARE_INTERFACE(IPlugin, "io.rup.IPlugin/1.0")

#if QT_VERSION < 0x050000
# define Q_PLUGIN_METADATA(...)
#endif

#if QT_VERSION >= 0x050000 && defined(Q_EXPORT_PLUGIN2)
# undef Q_EXPORT_PLUGIN2
# define Q_EXPORT_PLUGIN2(...)
#endif

#if QT_VERSION >= 0x050000
# define IMPORT_PLUGIN(name) Q_IMPORT_PLUGIN(name##Plugin)
#else
# define IMPORT_PLUGIN(name) Q_IMPORT_PLUGIN(name)
#endif

#endif // IPLUGIN_H_
