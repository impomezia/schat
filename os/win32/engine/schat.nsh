/* $Id: schat.nsh 3506 2013-02-15 18:02:08Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

!ifndef SCHAT_NSH_
!define SCHAT_NSH_

Var update

!include "engine\translations.nsh"
!AddPluginDir "contrib\plugins"

/*
 * Cохранения имени модуля.
 * Зависит от MUI2.nsh.
 */
!define Name "!insertmacro MUI_SET MOD_ID "


/*
 * Макрос для включения поддержки механизма сохранения/восстановления
 * состояния секции.
 * \param _STATE 1 - секция выбрана по умолчанию, 0 - не выбрана.
 */
!define State "!insertmacro SCHAT_MOD_STATE "
!macro SCHAT_MOD_STATE _STATE
  !ifdef SCHAT_INIT
    ${SectionState} "${MOD_ID}" ${_STATE} ${${MOD_ID}_idx}
  !endif

  !ifdef SCHAT_POST
    !insertmacro SAVE_SECTION ${${MOD_ID}_idx} "${MOD_ID}"
  !endif
!macroend


/*
 * Макрос контейнер для определения основной части модуля.
 * Используется парно: ${Body} и ${BodyEnd}.
 * Содержимое будет развёрнуто только при определении SCHAT_SECTIONS.
 * дополнительно устанавливает описание для секции.
 */
!define Body "!insertmacro SCHAT_MOD_BODY "
!define BodyEnd "!endif"
!macro SCHAT_MOD_BODY
  !ifdef SCHAT_DESC
    !insertmacro MUI_DESCRIPTION_TEXT ${${MOD_ID}_idx} "$(desc_${MOD_ID})"
  !endif
  !ifdef SCHAT_SECTIONS
!macroend


/*
 * Парные макросы для обвёртки стандартной секция для модуля.
 * Используется парно: ${Section} и ${SectionEnd}
 * и должны находиться внутри ${Body}.
 */
!define Section "!insertmacro SCHAT_SECTION "
!macro SCHAT_SECTION
  Section "$(sec_${MOD_ID})" ${MOD_ID}_idx
    SetOverwrite on
!macroend

!define SectionEnd "!insertmacro SCHAT_SECTION_END "
!macro SCHAT_SECTION_END
  SectionEnd
!macroend


!define Uninstall "!insertmacro SCHAT_MOD_UNINSTALL "
!define UninstallEnd "!endif"
!macro SCHAT_MOD_UNINSTALL
  !ifdef SCHAT_UNINSTALL
!macroend


/*
 * Управляет выбором секции.
 */
!define SectionState "!insertmacro SectionState"
!macro SectionState _KEY _DEF _SEC
  Push $0
  ClearErrors
  ReadIniStr $0 "$INSTDIR\uninstall.ini" "${SCHAT_NAME}" "${_KEY}"
  ${Unless} ${Errors}
    ${If} $0 != 1
    ${AndIf} $0 != 0
      StrCpy $0 ${_DEF}
    ${EndIf}
  ${Else}
    StrCpy $0 ${_DEF}
  ${EndUnless}

  ${If} $0 == 1
    !insertmacro SelectSection ${_SEC}
  ${Else}
    !insertmacro UnselectSection ${_SEC}
  ${EndIf}

  Pop $0
!macroend


/*
 * Сохраняет статус секции.
 */
!macro SAVE_SECTION _SEC _KEY
  ${If} ${SectionIsSelected} ${_SEC}
    WriteINIStr "$INSTDIR\uninstall.ini" "${SCHAT_NAME}" "${_KEY}" 1
  ${Else}
    WriteINIStr "$INSTDIR\uninstall.ini" "${SCHAT_NAME}" "${_KEY}" 0
  ${EndIf}
!macroend


/*
 * Обработка ключа командной строки "-update".
 * Путь установки становится на один уровень выше расположения exe файла.
 */
!macro UPDATE_CMD
  ${GetParameters} $R0
  ${GetOptionsS} $R0 "-update" $R0
  ${Unless} ${Errors}
    ${GetParent} "$EXEDIR" $R0
    StrCpy $INSTDIR $R0
    StrCpy $update true
  ${Else}
    StrCpy $update false
    !if ${SCHAT_CHECK_RUN} == 1
     call findRunningChat
    !endif
  ${EndUnless}
!macroend


/**
 * Завершает все процессы с указаным именем
 */
!macro KILL_ALL _NAME
 !if ${SCHAT_CHECK_RUN} == 1
  Push $R0
  StrCpy $R0 1
  ${While} $R0 == 1
    KillProcDLL::KillProc "${_NAME}"
    Pop $R0
    Sleep 500
    FindProcDLL::FindProc "${_NAME}"
    Pop $R0
  ${EndWhile}
  Pop $R0
 !endif
!macroend


/*
* Выводим `MessageBox` если чат запущен.
*/
!macro findRunningChat
  ${Unless} ${Silent}
    newcheck:
    FindProcDLL::FindProc "schat2.exe"
    Pop $R0
    ${If} $R0 == 1 
      MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(STR400)" IDRETRY newcheck
      Quit
    ${EndIf}
  ${EndUnless}

  !insertmacro KILL_ALL "schat2.exe"
!macroend

!macro FIND_RUNNING
 !if ${SCHAT_CHECK_RUN} == 1
  Function findRunningChat
    !insertmacro findRunningChat
  FunctionEnd
 !endif
!macroend

!macro UN_FIND_RUNNING
 !if ${SCHAT_CHECK_RUN} == 1
 !ifdef Core
   Function un.findRunningChat
    !insertmacro findRunningChat
   FunctionEnd
  !endif
 !endif
!macroend

!endif /* SCHAT_NSH_ */
