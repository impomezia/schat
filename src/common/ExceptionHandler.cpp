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

#include "ExceptionHandler.h"
#include "version.h"

#include <DbgHelp.h>
#include <time.h>

typedef BOOL WINAPI MINIDUMPWRITEDUMP(
  IN  HANDLE hProcess,
  IN  DWORD ProcessId,
  IN  HANDLE hFile,
  IN  MINIDUMP_TYPE DumpType,
  IN  PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL
  IN  PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL
  IN  PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL
);

static wchar_t fileName[MAX_PATH + 31];

void initExceptionHandler()
{
  memset(fileName, 0, sizeof(fileName));

  GetModuleFileNameW(NULL, fileName, sizeof(fileName) - 1);
  for (int i = lstrlenW(fileName) - 1; i >= 0; i--) {
    if ('.' == fileName[i]) {
      fileName[i] = 0;
      break;
    }
  }

  lstrcatW(fileName, L"-");
  lstrcpyW(fileName + lstrlenW(fileName), TEXT(SCHAT_VERSION));
  lstrcatW(fileName, L"-");
  lstrcpynW(fileName + lstrlenW(fileName), TEXT(GIT_REVISION), 8);
  lstrcatW(fileName, L"-");

  SetUnhandledExceptionFilter(exceptionFilter);
}


LONG WINAPI exceptionFilter(EXCEPTION_POINTERS *pExceptionInfo)
{
  LONG result = EXCEPTION_CONTINUE_SEARCH;

  HMODULE hDll = LoadLibraryW(L"DBGHELP.DLL");
  if (!hDll)
    return result;

  MINIDUMPWRITEDUMP *pDump = reinterpret_cast<MINIDUMPWRITEDUMP*>(GetProcAddress(hDll, "MiniDumpWriteDump"));
  if (!pDump)
    return result;

  MINIDUMP_EXCEPTION_INFORMATION exInfo = { GetCurrentThreadId(), pExceptionInfo, FALSE };

  const time_t now = time(0);
  struct tm timeinfo;
  gmtime_s(&timeinfo, &now);

  wcsftime(fileName + lstrlenW(fileName), 16, L"%Y%m%d-%H%M%S", &timeinfo);
  lstrcatW(fileName, L".dmp");

  HANDLE hFile = CreateFileW(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
    return result;

  pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &exInfo, NULL, NULL);
  CloseHandle(hFile);

  for (int i = lstrlenW(fileName) - 1; i >= 0; i--) {
    if ('\\' == fileName[i] || '/' == fileName[i]) {
      fileName[i] = 0;
      break;
    }
  }

  lstrcatW(fileName, L"\\crashreport.exe");
  ShellExecuteW(NULL, L"open", fileName, NULL, NULL, SW_SHOWNORMAL);

  return EXCEPTION_EXECUTE_HANDLER;
}
