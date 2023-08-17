#pragma once
#ifndef __WINTHREAD_HPP__
#define __WINTHREAD_HPP__

#include <minwindef.h>
#include <minwinbase.h>
#include <handleapi.h>
#include <processthreadsapi.h>
#include "winexc.hpp"

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

EXTERN_C DECLSPEC_IMPORT DWORD WINAPI WaitForSingleObject (HANDLE hHandle, DWORD dwMilliseconds);
EXTERN_C DECLSPEC_IMPORT VOID WINAPI EnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection);
EXTERN_C DECLSPEC_IMPORT VOID WINAPI LeaveCriticalSection (LPCRITICAL_SECTION lpCriticalSection);

namespace Windows {

class SysThread {
protected:
    HANDLE _handle;
    bool _suspended;
public:
    SysThread(
        LPSECURITY_ATTRIBUTES lpThreadAttributes,
        SIZE_T dwStackSize,
        LPTHREAD_START_ROUTINE lpStartAddress,
        PVOID lpParameter,
        DWORD dwCreationFlags,
        PDWORD lpThreadId
    ) {
        HANDLE thread = CreateThread(
            lpThreadAttributes,
            dwStackSize,
            lpStartAddress,
            lpParameter,
            dwCreationFlags,
            lpThreadId
        );
        ASSERT_THROW_WINA(thread != NULL, "failed to create thread");
        this->_handle = thread;
        this->_suspended = (dwCreationFlags & CREATE_SUSPENDED) ? true : false;
    }
    SysThread(LPTHREAD_START_ROUTINE func, LPVOID argptr, DWORD stack_size = 0, bool start = false):
        SysThread(NULL, stack_size, func, argptr, (start ? 0 : CREATE_SUSPENDED), NULL) {}
    EXCEPTABLE_DTOR(SysThread,
        this->timeout_kill(1000);
        BOOL success = CloseHandle(this->_handle);
        ASSERT_THROW_WINA(success, "failed to destroy thread");
    )
    bool exited(void) {
        DWORD exit_code;
        BOOL success = GetExitCodeThread(this->_handle, &exit_code);
        ASSERT_THROW_WINA(success, "failed to query thread status");
        return exit_code != STILL_ACTIVE;
    }
    bool suspended(void) {
        return this->_suspended;
    }
    bool get_state(void) {
        return (!this->exited()) && (!this->_suspended);
    }
    void filp_state(void) {
        if (this->get_state()) {
            DWORD code = SuspendThread(this->_handle);
            ASSERT_THROW_WINA(code != -1, "failed to suspend thread");
            this->_suspended = true;
        } else {
            DWORD code = ResumeThread(this->_handle);
            ASSERT_THROW_WINA(code != -1, "failed to resume thread");
            this->_suspended = false;
        }
    }
    void set_state(bool new_state) {
        if (this->get_state() != new_state) {
            if (this->exited())
                throw std::runtime_error("try to launch an exited thread");
            this->filp_state();
        }
    }
    void kill(void) {
        bool success = TerminateThread(this->_handle, 0);
        ASSERT_THROW_WINA(success, "failed to kill thread");
    }
    void timeout_kill(DWORD timeout) {
        DWORD result = this->join(timeout);
        if (result == WAIT_TIMEOUT) this->kill();
    }
    DWORD join(DWORD timeout = INFINITE) {
        DWORD result = WaitForSingleObject(this->_handle, timeout);
        ASSERT_THROW_WINA(result != WAIT_FAILED, "failed to kill thread");
        return result;
    }
};


// RAII. Lock a scope.
// if initialized with NULL, do nothing
class SysCriticalScope {
protected:
    CRITICAL_SECTION* _cs;
public:
    SysCriticalScope(CRITICAL_SECTION* pSection) {
        this->_cs = pSection;
        if (this->_cs) EnterCriticalSection(this->_cs);
    }
    ~SysCriticalScope() {
        if (this->_cs) LeaveCriticalSection(this->_cs);
    }
};

} // namespace Windows

#endif // __WINTHREAD_HPP__
