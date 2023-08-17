#pragma once
#ifndef __TDEF_HPP__
#define __TDEF_HPP__

#include "tdef.h"

#ifdef UNICODE
#   define to_Tstring    std::to_wstring
#   define Tstring       std::wstring
#   define Tstring_view  std::wstring_view
#else
#   define to_Tstring    std::to_string
#   define Tstring       std::string
#   define Tstring_view  std::string_view
#endif // UNICODE

#define TSTR(LIT) Tstring_view(TEXT(LIT))

#ifdef UNICODE
#   define empty_Tstring empty_wstring
#else
#   define empty_Tstring empty_string
#endif

#ifdef UNICODE
#   define sTtoW sWtoW
#   define sTtoA sWtoA
#   define sTtoU sWtoU
#   define sWtoT sWtoW
#   define sAtoT sAtoW
#   define sUtoT sUtoW
#else
#   define sTtoW sAtoW
#   define sTtoA sAtoA
#   define sTtoU sAtoU
#   define sWtoT sWtoA
#   define sAtoT sAtoA
#   define sUtoT sUtoA
#endif

#endif // __TDEF_HPP__
