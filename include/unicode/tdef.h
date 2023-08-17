#pragma once
#ifndef __TDEF_H__
#define __TDEF_H__

#ifdef UNICODE
#   define Tfgetc       fgetwc
#   define Tfgets       fgetws
#   define Tfputc       fputwc
#   define Tfputs       fputws
#   define Tfprintf     fwprintf
#   define Tfscanf      fwscanf
#   define Tgetchar     getwchar
#   define Tgetc        getwc
#   define Tputchar     putwchar
#   define Tputc        putwc
#   define Tsprintf     swprintf
#   define Tsscanf      swscanf
#   define Tungetc      ungetwc
#   define Tvfprintf    vfwprintf
#   define Tvfscanf     vfwscanf
#   define Tvsprintf    vswprintf
#   define Tvsscanf     vswscanf
#   define Tvprintf     vwprintf
#   define Tvscanf      vwscanf
#   define Tstrcat      wcscat
#   define Tstrchr      wcschr
#   define Tstrcmp      wcscmp
#   define Tstrcoll     wcscoll
#   define Tstrcpy      wcscpy
#   define Tstrcspn     wcscspn
#   define Tstrftime    wcsftime
#   define Tstrlen      wcslen
#   define Tstrncat     wcsncat
#   define Tstrncmp     wcsncmp
#   define Tstrncpy     wcsncpy
#   define Tstrpbrk     wcspbrk
#   define Tstrrchr     wcsrchr
#   define Tstrspn      wcsspn
#   define Tstrstr      wcsstr
#   define Tstrtod      wcstod
#   define Tstrtof      wcstof
#   define Tstrtok      wcstok
#   define Tstrtold     wcstold
#   define Tstrtoll     wcstoll
#   define Tstrtol      wcstol
#   define Tstrtoull    wcstoull
#   define Tstrtoul     wcstoul
#   define Tstrxfrm     wcsxfrm
#   define Tprintf      wprintf
#   define Tscanf       wscanf
#else
#   define Tfgetc       fgetc
#   define Tfgets       fgets
#   define Tfputc       fputc
#   define Tfputs       fputs
#   define Tfprintf     fprintf
#   define Tfscanf      fscanf
#   define Tgetchar     getchar
#   define Tgetc        getc
#   define Tputchar     putchar
#   define Tputc        putc
#   define Tsprintf     sprintf
#   define Tsscanf      sscanf
#   define Tungetc      ungetc
#   define Tvfprintf    vfprintf
#   define Tvfscanf     vfscanf
#   define Tvsprintf    vsprintf
#   define Tvsscanf     vsscanf
#   define Tvprintf     vprintf
#   define Tvscanf      vscanf
#   define Tstrcat      strcat
#   define Tstrchr      strchr
#   define Tstrcmp      strcmp
#   define Tstrcoll     strcoll
#   define Tstrcpy      strcpy
#   define Tstrcspn     strcspn
#   define Tstrftime    strftime
#   define Tstrlen      strlen
#   define Tstrncat     strncat
#   define Tstrncmp     strncmp
#   define Tstrncpy     strncpy
#   define Tstrpbrk     strpbrk
#   define Tstrrchr     strrchr
#   define Tstrspn      strspn
#   define Tstrstr      strstr
#   define Tstrtod      strtod
#   define Tstrtof      strtof
#   define Tstrtok      strtok
#   define Tstrtold     strtold
#   define Tstrtoll     strtoll
#   define Tstrtol      strtol
#   define Tstrtoull    strtoull
#   define Tstrtoul     strtoul
#   define Tstrxfrm     strxfrm
#   define Tprintf      printf
#   define Tscanf       scanf
#endif // UNICODE

#ifdef UNICODE
#   define Tchar wchar_t
#   define empty_Tcstr empty_wcstr
#else
#   define Tchar char
#   define empty_Tcstr empty_cstr
#endif

#ifdef UNICODE
#   define TtoW WtoW
#   define TtoA WtoA
#   define TtoU WtoU
#   define WtoT WtoW
#   define AtoT AtoW
#   define UtoT UtoW
#else
#   define TtoW AtoW
#   define TtoA AtoA
#   define TtoU AtoU
#   define WtoT WtoA
#   define AtoT AtoA
#   define UtoT UtoA
#endif

#endif // __TDEF_HPP__
