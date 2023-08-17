#pragma once
#ifndef __WINEXC_HPP__
#define __WINEXC_HPP__

#include <stdexcept>
#include <winerror.h>
#include <errhandlingapi.h>

#define ASSERT_EXIT(expr, msg) if (!(expr)) { printf("Error: %s", msg); exit(EXIT_FAILURE); }
#define ASSERT_THROW_STD(expr, msg) if (!(expr)) {throw std::runtime_error(msg);}
#define ASSERT_THROW_WINA(expr, msg) if (!(expr)) {throw Windows::WinError<char>(GetLastError(), msg);}
#define ASSERT_THROW_WINW(expr, msg) if (!(expr)) {throw Windows::WinError<wchar_t>(GetLastError(), msg);}
#define ASSERT_THROW_WINT(expr, msg) if (!(expr)) {throw Windows::WinError<TCHAR>(GetLastError(),msg);}

#ifdef DEBUG
#define EXCEPTABLE_DTOR(class, code) \
    ~class() noexcept(false) { code }
#else
#define EXCEPTABLE_DTOR(class, code) \
    ~class() noexcept { \
        try { \
            code \
        } catch(...) { \
            printf("Exception happened in DTOR of %s but swallowed!", #class); \
        } \
    }
#endif

namespace Windows {

template <typename charType>
class WinError {
protected:
    DWORD _errcode;
    charType* _msg;
    size_t _msglen;
    charType* _what_content;
public:
    WinError(DWORD errcode, const charType* msg, size_t msglen) {
        this->_errcode = errcode;
        this->_msglen = msglen;
        this->_msg = (charType* )malloc((msglen + 1) * sizeof(charType));
        memcpy(this->_msg, msg, msglen * sizeof(charType));
        this->_msg[msglen] = '\0';
        this->_what_content = NULL;
    }
    WinError(DWORD errcode, const charType* msg):
        WinError(errcode, msg, strlen(msg)) {}
    WinError(DWORD errcode, const std::basic_string<charType>& msg):
        WinError(errcode, msg.data(), msg.length()) {}
    WinError(DWORD errcode, const std::basic_string_view<charType>& msg):
        WinError(errcode, msg.data(), msg.length()) {}
    ~WinError() {
        free(this->_msg);
        if (this->_what_content) free(this->_what_content);
    }
    int code(void) {
        return this->_errcode;
    }
    charType* what(void) {
        if (!this->_what_content) {
            this->_what_content = (charType* )malloc((this->_msglen + 1 + 32) * sizeof(charType));
            sprintf(this->_what_content, "[WinError %ld] %s", this->_errcode, this->_msg);
        }
        return this->_what_content;
    }
};

} // namespace Windows

#endif // __WINEXC_HPP__
