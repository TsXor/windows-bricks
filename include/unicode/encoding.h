#pragma once
#ifndef __ENCODING_H__
#define __ENCODING_H__

#include <stdlib.h>
#include <string.h>

#include <minwindef.h>
#include <stringapiset.h>


// in case you did not include Windows.h
#ifndef TEXT
#   ifdef UNICODE
#       define TEXT(LIT) L##LIT    
#   else
#       define TEXT(LIT) LIT
#   endif // UNICODE
#endif

#ifndef _TCHAR_DEFINED
#   ifdef UNICODE
#       define TCHAR wchar_t
#   else
#       define TCHAR char
#   endif // UNICODE
#endif

static const char* empty_cstr = "";
static const wchar_t* empty_wcstr = L"";

// W stands for WIDE, on windows it is utf-16
// A stands for ANSI, the binarily same ANSI string differ on different locales 
// U stands for UTF-8, universal on ALL system

// CRT API on windows and Windows API wants and returns W or A
// ALL literals have the same encoding as file
// argv from main() is A even when under codepage 65001

typedef struct str_cvt_ctx {
    union {
        char* as_char;
        wchar_t* as_wchar;
    } tmp_str;
    int tmp_len;
} str_cvt_ctx_t;

#define INIT_STR_CVT_CTX {NULL, 0}

inline int AtoW(const char* in_str, int in_len, wchar_t* out_str, int out_len, str_cvt_ctx_t* ctx) {
    return MultiByteToWideChar(CP_ACP, 0, in_str, in_len, out_str, out_len);
}

inline int WtoA(const wchar_t* in_str, int in_len, char* out_str, int out_len, str_cvt_ctx_t* ctx) {
    return WideCharToMultiByte(CP_ACP, 0, in_str, in_len, out_str, out_len, NULL, NULL);
}

inline int UtoW(const char* in_str, int in_len, wchar_t* out_str, int out_len, str_cvt_ctx_t* ctx) {
    return MultiByteToWideChar(CP_UTF8, 0, in_str, in_len, out_str, out_len);
}

inline int WtoU(const wchar_t* in_str, int in_len, char* out_str, int out_len, str_cvt_ctx_t* ctx) {
    return WideCharToMultiByte(CP_UTF8, 0, in_str, in_len, out_str, out_len, NULL, NULL);
}

inline int UtoA(const char* in_str, int in_len, char* out_str, int out_len, str_cvt_ctx_t* ctx) {
    if (!(ctx->tmp_len && ctx->tmp_str.as_wchar)) {
        // init ctx
        ctx->tmp_len = UtoW(in_str, in_len, NULL, 0, NULL);
        ctx->tmp_str.as_wchar = (wchar_t* )malloc(ctx->tmp_len * sizeof(wchar_t));
        UtoW(in_str, in_len, ctx->tmp_str.as_wchar, ctx->tmp_len, NULL);
    }
    int ret = WtoA(ctx->tmp_str.as_wchar, ctx->tmp_len, out_str, out_len, NULL);
    
    if (out_len && (ctx->tmp_len || ctx->tmp_str.as_wchar)) {
        if (ctx->tmp_str.as_wchar) free(ctx->tmp_str.as_wchar);
        ctx->tmp_str.as_wchar = NULL; ctx->tmp_len = 0;
    }
    return ret;
}

inline int AtoU(const char* in_str, int in_len, char* out_str, int out_len, str_cvt_ctx_t* ctx) {
    if (!(ctx->tmp_len && ctx->tmp_str.as_wchar)) {
        // init ctx
        ctx->tmp_len = AtoW(in_str, in_len, NULL, 0, NULL);
        ctx->tmp_str.as_wchar = (wchar_t* )malloc(ctx->tmp_len * sizeof(wchar_t));
        AtoW(in_str, in_len, ctx->tmp_str.as_wchar, ctx->tmp_len, NULL);
    }
    int ret = WtoU(ctx->tmp_str.as_wchar, ctx->tmp_len, out_str, out_len, NULL);
    
    if (out_len && (ctx->tmp_len || ctx->tmp_str.as_wchar)) {
        if (ctx->tmp_str.as_wchar) free(ctx->tmp_str.as_wchar);
        ctx->tmp_str.as_wchar = NULL; ctx->tmp_len = 0;
    }
    return ret;
}

inline int WtoW(const wchar_t* in_str, int in_len, wchar_t* out_str, int out_len) {
    if (out_len) memcpy(out_str, in_str, in_len * sizeof(wchar_t));
    return in_len;
}

inline int AtoA(const char* in_str, int in_len, char* out_str, int out_len) {
    if (out_len) memcpy(out_str, in_str, in_len * sizeof(char));
    return in_len;
}

inline int UtoU(const char* in_str, int in_len, char* out_str, int out_len) {
    if (out_len) memcpy(out_str, in_str, in_len * sizeof(char));
    return in_len;
}

#endif // __ENCODING_H__
