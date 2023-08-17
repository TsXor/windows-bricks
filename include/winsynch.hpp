#pragma once
#ifndef __WINSYNCH_HPP__
#define __WINSYNCH_HPP__

#include <minwindef.h>
#include <minwinbase.h>
#include <handleapi.h>
#include <synchapi.h>
#include <securitybaseapi.h>
#include <stdio.h>
#include "winexc.hpp"
#include "unicode/encoding.hpp"
#include "unicode/tdef.hpp"


//#define pass // 写Python写的

#ifndef INFINITE
#define INFINITE 0xffffffff
#endif

#ifndef CREATE_SUSPENDED
#define CREATE_SUSPENDED 0x4
#endif

#ifndef WAIT_FAILED
#define WAIT_FAILED ((DWORD)0xffffffff)
#endif

#ifndef WAIT_ABANDONED
#define WAIT_ABANDONED ((STATUS_ABANDONED_WAIT_0) + 0)
#endif


EXTERN_C DECLSPEC_IMPORT HLOCAL WINAPI LocalAlloc (UINT uFlags, SIZE_T uBytes);
EXTERN_C DECLSPEC_IMPORT HLOCAL WINAPI LocalFree (HLOCAL hMem);


namespace Windows {

class KernelSyncObject {
protected:
    HANDLE _handle;
    std::wstring* _name = nullptr;
public:
    KernelSyncObject(bool create, const std::wstring_view& name,
                     HANDLE (*fn_create)(LPSECURITY_ATTRIBUTES, WINBOOL, const wchar_t*),
                     HANDLE (*fn_open)(DWORD, WINBOOL, const wchar_t*) ) {
        bool is_anon = name.empty();
        HANDLE hd = NULL;
        if (is_anon) {
            hd = fn_create(NULL, FALSE, NULL);
        } else {
            this->_name = new std::wstring(L"Global\\");
            *(this->_name) += name;
            if (create) {
                bool op_success;
                PSECURITY_DESCRIPTOR sd = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
                ASSERT_THROW_WINA(sd, "kernel sync object: failed to LocalAlloc space for SECURITY_DESCRIPTOR");
                op_success = InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION);
                ASSERT_THROW_WINA(op_success, "kernel sync object: failed to InitializeSecurityDescriptor");
                op_success = SetSecurityDescriptorDacl(sd, TRUE, NULL, FALSE);
                ASSERT_THROW_WINA(op_success, "kernel sync object: failed to SetSecurityDescriptorDacl");
                SECURITY_ATTRIBUTES sa;
                sa.nLength = sizeof(sa);
                sa.lpSecurityDescriptor = sd;
                sa.bInheritHandle = FALSE;
                hd = fn_create(&sa, FALSE, this->_name->c_str());
            } else {
                hd = fn_open(SYNCHRONIZE, FALSE, this->_name->c_str());
            }
        }
        ASSERT_THROW_WINA(hd != NULL, "failed to create kernel sync object");
        this->_handle = hd;
    }
    EXCEPTABLE_DTOR(KernelSyncObject,
        if (this->_name) delete this->_name;
        BOOL success = CloseHandle(this->_handle);
        ASSERT_THROW_WINA(success, "failed to destroy kernel sync object");
    )
    std::wstring_view get_name(void) {
        return this->_name ? std::wstring_view(*(this->_name)) : empty_wstring_view;
    }
};

class SysLock : public KernelSyncObject {
public:
    SysLock(bool create, const std::wstring_view& name):
        KernelSyncObject(create, name, CreateMutexW, OpenMutexW) {}
    SysLock():
        SysLock(true, WSTR("")) {}

    void acquire(void) {
        DWORD result = WaitForSingleObject(this->_handle, INFINITE);
        ASSERT_THROW_WINA(result != WAIT_FAILED, "failed to acquire lock");
    }
    void release(void) {
        BOOL success = ReleaseMutex(this->_handle);
        ASSERT_THROW_WINA(success, "failed to release lock");
    }
};

template <WINBOOL bManualReset>
HANDLE __fixparam_CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, WINBOOL bInitialState, const wchar_t* lpName) {
    return CreateEventW(lpEventAttributes, bManualReset, bInitialState, lpName);
}

template <WINBOOL bManualReset>
class SysEvent : public KernelSyncObject {
public:
    SysEvent(bool create, const std::wstring_view& name):
        KernelSyncObject(create, name, __fixparam_CreateEventW<bManualReset>, OpenEvent) {}
    SysEvent():
        SysEvent(true, WSTR("")) {}

    void set(void) {
        BOOL success = SetEvent(this->_handle);
        ASSERT_THROW_WINA(success, "failed to set event");
    }
    void reset(void) {
        BOOL success = ResetEvent(this->_handle);
        ASSERT_THROW_WINA(success, "failed to reset event");
    }
    void wait(void){
        DWORD result = WaitForSingleObject(this->_handle, INFINITE);
        ASSERT_THROW_WINA(result != WAIT_FAILED, "failed to wait for event");
    }
};

using SysAutoResetEvent = SysEvent<TRUE>;
using SysManualResetEvent = SysEvent<FALSE>;


/*
    由于Windows.h中的Interlocked*系列函数会在C++下十分“智能”地变成同一个函数的重载，
    所以Windows.h不能在命名空间里include。
    要在命名空间里include，需要定义某个标志宏来禁用这些函数，
    所以下面这段就也不能用了。
*/

/*
#ifdef _MSC_VER
#define IE_SWAPPABLE(T) (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8)
template <typename T> requires (sizeof(T) == 1)
T InterlockedExchangeAny(T* t, T v) {
    return InterlockedExchange8((int8_t *)t, (int8_t)v);
}
template <typename T> requires (sizeof(T) == 2)
T InterlockedExchangeAny(T* t, T v) {
    return InterlockedExchange16((int16_t *)t, (int16_t)v);
}
template <typename T> requires (sizeof(T) == 4)
T InterlockedExchangeAny(T* t, T v) {
    return InterlockedExchange((long *)t, (long)v);
}
template <typename T> requires (sizeof(T) == 8)
T InterlockedExchangeAny(T* t, T v) {
    return InterlockedExchange64((int64_t *)t, (int64_t)v);
}
#else
#define IE_SWAPPABLE(T) (sizeof(T) == 4 || sizeof(T) == 8)
template <typename T> requires (sizeof(T) == 4)
T InterlockedExchangeAny(T* t, T v) {
    return InterlockedExchange((__LONG32 *)t, (__LONG32)v);
}
template <typename T> requires (sizeof(T) == 8)
T InterlockedExchangeAny(T* t, T v) {
    return InterlockedExchange64((__int64 *)t, (__int64)v);
}
#endif

#define IS_SMALL_TYPE(T) (sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8)

#ifdef _MSC_VER
template <typename T>
union __ie_min_align {
    T real;
    char align;
};
#else
template <typename T>
union __ie_min_align {
    T real;
    long align;
};
#endif

template <typename T>
class InterlockedValue {
protected:
    __ie_min_align<T> _value;
public:
    template <typename = void> requires (IS_SMALL_TYPE(T))
    InterlockedValue(T value) {
        this->_value.real = value;
    }
    template <typename = void> requires (IS_SMALL_TYPE(T))
    void operator=(const T val) {
        __ie_min_align<T> val_align;
        val_align.real = val;
        if (IE_SWAPPABLE(T)) InterlockedExchangeAny(&(this->_value.real), val_align.real);
        else InterlockedExchangeAny(&(this->_value.align), val_align.align);
    }
    template <typename = void> requires (IS_SMALL_TYPE(T))
    operator T() {
        __ie_min_align<T> ret;
        if (IE_SWAPPABLE(T)) InterlockedExchangeAny(&(ret.real), this->_value.real);
        else InterlockedExchangeAny(&(ret.align), this->_value.align);
        return ret.real;
    }
};

template <typename T>
class InterlockedPointerValue {
protected:
    T* _value;
public:
    // WARNING: This can only protect the pointer itself.
    InterlockedPointerValue(T* value) {
        this->_value = value;
    }
    void operator=(const T* val) {
        void* dst = this->_value;
        InterlockedExchangePointer(&dst, (void* )val);
    }
    operator T*() {
        void* ret;
        InterlockedExchangePointer(&ret, (void* )(this->_value));
        return (T* )ret;
    }
    bool operator!() {
        T* val = *this;
        return val ? false : true;
    }
};
*/

} // namespace Windows

#endif // __WINSYNCH_HPP__
