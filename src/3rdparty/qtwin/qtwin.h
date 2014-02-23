/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
 * Copyright (c) 2013 Ivan Vizir <define-true-false@yandex.com>
 * Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * <http://labs.qt.nokia.com/2009/09/15/using-blur-behind-on-windows/>
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

#ifndef QTWIN_H_
#define QTWIN_H_

#include <QColor>

class WindowNotifier;

class QtWin
{
public:
    static bool extendFrameIntoClientArea(QWidget *widget, int left = -1, int top = -1, int right = -1, int bottom = -1);
    static bool isCompositionEnabled();
    static QColor realColorizationColor();
    static void resetExtendedFrame(QWidget *widget);
};

#endif // QTWIN_H_
