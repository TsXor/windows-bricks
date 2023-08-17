// https://learn.microsoft.com/zh-cn/windows/console/registering-a-control-handler-function
// https://github.com/microsoft/Windows-classic-samples/blob/1d363ff4bd17d8e20415b92e2ee989d615cc0d91/Samples/RadialController/cpp/RadialController.cpp

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "Windows.h"
#include "../include/winsynch.hpp"
#include "../include/exit_event.h"

static exit_info_t __exit_info = {false, EXIT_CON, 0};
static Windows::SysLock __exit_info_rwlock;
static HWND __hidden_window_hwnd = NULL;

static BOOL WINAPI __CtrlHandler(DWORD fdwCtrlType) {
    __exit_info_rwlock.acquire();
    if (!__exit_info.available) {
        __exit_info.available = true;
        __exit_info.type = EXIT_CON;
        __exit_info.arg.fdwCtrlType = fdwCtrlType;
    }
    __exit_info_rwlock.release();
    // let window close later
    PostMessage(__hidden_window_hwnd, WM_CLOSE, 0, 0);
    return TRUE;
}

static LRESULT CALLBACK __HiddenWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            __exit_info_rwlock.acquire();
            if (!__exit_info.available) {
                __exit_info.available = true;
                __exit_info.type = EXIT_WM;
                __exit_info.arg.fdwCtrlType = lParam;
            }
            __exit_info_rwlock.release();
            return 0;
        case WM_QUERYENDSESSION:
            // let window close later
            // SendMessage may deadlock here?
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            return FALSE; // STOP! LET ME FINALIZE!
        case WM_ENDSESSION:
            return 0; // We have handled this message.
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

static BOOL __init_hidden_window(void) {
    WNDCLASS sampleClass{ 0 };
    sampleClass.lpszClassName = TEXT("CtrlHandlerSampleClass");
    sampleClass.lpfnWndProc = __HiddenWindowProc;

    ATOM retcode_registerclass = RegisterClass(&sampleClass);
    if (!retcode_registerclass) return FALSE;

    __hidden_window_hwnd = CreateWindowEx(
        0,
        sampleClass.lpszClassName,
        TEXT("Hidden Window to Receive System Messages"),
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );
    if (!__hidden_window_hwnd) return FALSE;

    ShowWindow(__hidden_window_hwnd, SW_HIDE);
    return TRUE;
}

extern "C" {

bool init_exit_handler(void) {
    bool init_wm_success = __init_hidden_window();
    bool init_con_success = SetConsoleCtrlHandler(__CtrlHandler, TRUE);

    bool success = init_wm_success && init_con_success;
    if (!success) {
        if (init_wm_success) {
            // let window close later
            PostMessage(__hidden_window_hwnd, WM_CLOSE, 0, 0);
        }
        if (init_con_success) {
            // no cleaning for SetConsoleCtrlHandler
        }
    }
    
    return success;
}

void wait_for_exit(void) {
    // Pump message loop for the window we created.
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

exit_info_t get_exit_info(void) {
    __exit_info_rwlock.acquire();
    exit_info_t ret = __exit_info;
    __exit_info_rwlock.release();
    return ret;
}

} // extern "C"
