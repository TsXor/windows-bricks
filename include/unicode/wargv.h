#pragma once
#ifndef __WARGV_H__
#define __WARGV_H__

#include <minwindef.h>
#include <minwinbase.h>

EXTERN_C DECLSPEC_IMPORT LPWSTR* WINAPI CommandLineToArgvW(LPCWSTR lpCmdLine, int* pNumArgs);
EXTERN_C DECLSPEC_IMPORT LPWSTR WINAPI GetCommandLineW(void);
EXTERN_C DECLSPEC_IMPORT HLOCAL WINAPI LocalAlloc (UINT uFlags, SIZE_T uBytes);
EXTERN_C DECLSPEC_IMPORT HLOCAL WINAPI LocalFree(HLOCAL hMem);

inline wchar_t** get_wargv(void) {
    int argc;
    return CommandLineToArgvW(GetCommandLineW(), &argc);
}

inline void free_wargv(wchar_t** ptr) {
    LocalFree(ptr);
}

#endif // __WARGV_H__
