/* $Id: update.nsh 2218 2012-01-27 01:48:33Z IMPOMEZIA $
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
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

!ifndef UPDATE_NSH_
!define UPDATE_NSH_

/**
 * Функции
 */
!macro UPDATE_ENGINE_FUNCTIONS
  !ifdef Core
    !insertmacro un.GetParameters
    !insertmacro un.GetOptionsS
    !insertmacro un.WordFind
  !endif
!macroend


!ifdef Core
!include "FileFunc.nsh"
!include "WordFunc.nsh"

Var mode
Var cmdparams
Var run
Var clean
Var files

/**
 * Инициализация
 */
!macro UPDATE_ENGINE_INIT
  ${un.GetParameters} $cmdparams
  ${un.GetOptionsS} $cmdparams "-update" $R0
  ${Unless} ${Errors}
    StrCpy $mode "update"

    !insertmacro UPDATE_ENGINE_CMDOPTION $run   "-run"
    !insertmacro UPDATE_ENGINE_CMDOPTION $clean "-clean"

    ReadIniStr $files "$INSTDIR\schat.conf" "Updates" "Files"
    ${If} $files == ""
      !insertmacro UPDATE_ENGINE_FAIL
    ${EndIf}

    ${un.WordFind} "$files" ", " "#" $0

    Banner::show /NOUNLOAD /set 76 "${SCHAT_NAME}" "Processing Update..."

    Sleep 1000
    !if ${SCHAT_CHECK_RUN} == 1
     call un.findRunningChat
    !endif
    
    ${If} $files == $0 ; один файл
      !insertmacro UPDATE_ENGINE_EXEC $files
    ${Else}
      ${For} $1 1 $0
        ${un.WordFind} $files ", " "+$1" $R0
        !insertmacro UPDATE_ENGINE_EXEC $R0
      ${Next}
    ${EndIf}

    !insertmacro UPDATE_ENGINE_DONE
  ${Else}
    StrCpy $mode "normal"
    !if ${SCHAT_CHECK_RUN} == 1
     call un.findRunningChat
    !endif
  ${EndUnless}
!macroend


/**
 * Запуск
 */
!macro UPDATE_ENGINE_EXEC _FILENAME
  ${If} ${FileExists} "$INSTDIR\updates\${_FILENAME}"
    ExecWait `"$INSTDIR\updates\${_FILENAME}" /S -update`
  ${Else}
    !insertmacro UPDATE_ENGINE_FAIL
  ${EndIf}
!macroend


/**
 * Установка опции из командной строки
 * Устанавливает переменную в `1` если ключ присутствует в командной строке, иначе `0`.
 */
!macro UPDATE_ENGINE_CMDOPTION _VAR _KEY
  Push $R0
  ${un.GetOptionsS} $cmdparams "${_KEY}" $R0
  ${Unless} ${Errors}
    StrCpy ${_VAR} 1
  ${Else}
    StrCpy ${_VAR} 0
  ${EndIf}
  Pop $R0
!macroend


/**
 * Завершение в результате ошибки
 */
!macro UPDATE_ENGINE_FAIL
  WriteINIStr "$INSTDIR\schat.conf" "Updates" "ReadyToInstall" "false"

  Banner::destroy
  ${If} $run == 1
    Exec `"$INSTDIR\schat.exe"`
  ${EndIf}
  Quit
!macroend


/**
 * Успешное завершение
 */
!macro UPDATE_ENGINE_DONE
  WriteINIStr "$INSTDIR\schat2.conf" "Updates" "ReadyToInstall" "false"

  ${If} $clean == 1
  ${EndIf}

  !insertmacro UPDATE_ENGINE_CLEAN

  Banner::destroy
  ${If} $run == 1
    Exec `"$INSTDIR\schat2.exe"`
  ${EndIf}

  Quit
!macroend


/**
 * Очистка
 */
!macro UPDATE_ENGINE_CLEAN
  ${If} $clean == 1  
    ${un.WordFind} "$files" ", " "#" $0

    ${If} $files == $0 ; один файл
      Delete "$INSTDIR\updates\$files"
    ${Else}
      ${For} $1 1 $0
        ${un.WordFind} $files ", " "+$1" $R0
        Delete "$INSTDIR\updates\$R0"
      ${Next}
    ${EndIf}

    Delete "$INSTDIR\updates\mirror.xml"
  ${EndIf}
!macroend

!endif
!endif /* UPDATE_NSH_ */
