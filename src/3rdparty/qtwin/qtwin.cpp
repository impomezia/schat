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

#include <QWidget>
#include <QSettings>

#include <qt_windows.h>

#if defined(Q_CC_MSVC)
# include <uxtheme.h>
#else
typedef struct _MARGINS {
  int cxLeftWidth;
  int cxRightWidth;
  int cyTopHeight;
  int cyBottomHeight;
} MARGINS, *PMARGINS;
#endif

#include "qtwin.h"

void qt_winextras_init();

typedef HRESULT (STDAPICALLTYPE *DwmIsCompositionEnabled_t)(BOOL *);
static DwmIsCompositionEnabled_t pDwmIsCompositionEnabled = 0;

typedef HRESULT (STDAPICALLTYPE *DwmExtendFrameIntoClientArea_t)(HWND, const MARGINS *);
static DwmExtendFrameIntoClientArea_t pDwmExtendFrameIntoClientArea = 0;

bool QtWin::isCompositionEnabled()
{
    qt_winextras_init();

    BOOL enabled = false;
    if (pDwmIsCompositionEnabled)
        pDwmIsCompositionEnabled(&enabled);

    return enabled;
}


bool QtWin::extendFrameIntoClientArea(QWidget *widget, int left, int top, int right, int bottom)
{
    qt_winextras_init();

    MARGINS margins = {left, right, top, bottom};
    if (pDwmExtendFrameIntoClientArea)
        return pDwmExtendFrameIntoClientArea(reinterpret_cast<HWND>(widget->winId()), &margins);

    return false;
}


QColor QtWin::realColorizationColor()
{
    bool ok = false;

    QSettings registry(QLatin1String("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\DWM"), QSettings::NativeFormat);
    const QRgb color = registry.value(QLatin1String("ColorizationColor")).toUInt(&ok);
    return ok ? QColor::fromRgba(color) : QColor();
}


void QtWin::resetExtendedFrame(QWidget *widget)
{
    extendFrameIntoClientArea(widget, 0, 0, 0, 0);
}


void qt_winextras_init()
{
    static bool initialized = false;
    if (initialized)
        return;

    HMODULE dwmapi = LoadLibraryW(L"dwmapi.dll");
    if (dwmapi) {
        pDwmExtendFrameIntoClientArea = (DwmExtendFrameIntoClientArea_t) GetProcAddress(dwmapi, "DwmExtendFrameIntoClientArea");
        pDwmIsCompositionEnabled = (DwmIsCompositionEnabled_t) GetProcAddress(dwmapi, "DwmIsCompositionEnabled");
    }
}

