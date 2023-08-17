#pragma once
#ifndef __ENCODING_HPP__
#define __ENCODING_HPP__

#include <string>
#include "encoding.h"

#define STR(LIT)   std::string(LIT)
#define WSTR(LIT)  std::wstring(L##LIT)
#define STRV(LIT)  std::string_view(LIT)
#define WSTRV(LIT) std::wstring_view(L##LIT)
const auto empty_string_view = STRV("");
const auto empty_wstring_view = WSTRV("");

// on stack
template <typename Tci, typename Tco>
inline std::basic_string<Tco> str_enc_cvt(const std::basic_string_view<Tci>& in_str,
                                   int (*cvf)(const Tci*, int, Tco*, int, str_cvt_ctx_t*)) {
    str_cvt_ctx_t cv_ctx = INIT_STR_CVT_CTX; auto cv_ctx_ptr = &cv_ctx;
    int out_len = cvf(in_str.data(), in_str.length(), NULL, 0, cv_ctx_ptr);
    std::basic_string<Tco> out_str;
#ifdef __cpp_lib_string_resize_and_overwrite
    out_str.resize_and_overwrite(out_len,
        [&in_str, cvf, cv_ctx_ptr, out_len](Tco* buf, size_t buf_size) {
            cvf(in_str.data(), in_str.length(), buf, out_len, cv_ctx_ptr);
            return out_len;
        }
    );
#else
    out_str.resize(out_len);
    cvf(in_str.data(), in_str.length(), &(out_str.front()), out_len, cv_ctx_ptr);
#endif
    return out_str;
}

template <typename Tci, typename Tco>
inline std::basic_string<Tco> str_enc_cvt(const std::basic_string<Tci>& in_str,
                                   int (*cvf)(const Tci*, int, Tco*, int, str_cvt_ctx_t*)) {
    return str_enc_cvt<Tci, Tco>(std::basic_string_view<Tci>(in_str), cvf);
}

// on heap
template <typename Tci, typename Tco>
inline void str_enc_cvt(const std::basic_string<Tci>* in_str, std::basic_string<Tco>* out_str,
                 int (*cvf)(const Tci*, int, Tco*, int, str_cvt_ctx_t*)) {
    str_cvt_ctx_t cv_ctx = INIT_STR_CVT_CTX; auto cv_ctx_ptr = &cv_ctx;
    int out_len = cvf(in_str->data(), in_str->length(), NULL, 0, cv_ctx_ptr);
#ifdef __cpp_lib_string_resize_and_overwrite
    out_str->resize_and_overwrite(out_len,
        [&in_str, cvf, cv_ctx_ptr, out_len](Tco* buf, size_t buf_size) {
            cvf(in_str->data(), in_str->length(), buf, out_len, cv_ctx_ptr);
            return out_len;
        }
    );
#else
    out_str->resize(out_len);
    cvf(in_str->data(), in_str->length(), &(out_str->front()), out_len, cv_ctx_ptr);
#endif
}

// on stack
template <typename T>
inline std::basic_string<T> str_copy(const std::basic_string_view<T>& in_str) {
    return std::basic_string<T>(in_str);
}

// on heap
template <typename T>
inline void str_copy(const std::basic_string<T>* in_str, std::basic_string<T>* out_str) {
#ifdef __cpp_lib_string_resize_and_overwrite
    out_str->resize_and_overwrite(in_str->length(),
        [&in_str](T* buf, size_t buf_size) {
            memcpy(buf, in_str->data(), in_str->length() * sizeof(T));
            return in_str->length();
        }
    );
#else
    out_str->resize(in_str->length());
    memcpy(&(out_str->front()), in_str->data(), in_str->length() * sizeof(T));
#endif
}

#define sWtoA(...) str_enc_cvt<wchar_t, char>(__VA_ARGS__, WtoA)
#define sAtoW(...) str_enc_cvt<char, wchar_t>(__VA_ARGS__, AtoW) 
#define sWtoU(...) str_enc_cvt<wchar_t, char>(__VA_ARGS__, WtoU)
#define sUtoW(...) str_enc_cvt<char, wchar_t>(__VA_ARGS__, UtoW)
#define sUtoA(...) str_enc_cvt<char, char>(__VA_ARGS__, UtoA)
#define sAtoU(...) str_enc_cvt<char, char>(__VA_ARGS__, AtoU)
#define sWtoW(...) str_copy<wchar_t>(__VA_ARGS__)
#define sAtoA(...) str_copy<char>(__VA_ARGS__)
#define sUtoU(...) str_copy<char>(__VA_ARGS__)

#endif // __ENCODING_HPP__
