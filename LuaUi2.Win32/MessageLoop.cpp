#include "MessageLoop.h"

#include <Windows.h>

using luaUi::MessageLoop;

MessageLoop::MessageLoop() = default;

void MessageLoop::run() {
    exitCode = -1;

    while (true) {
        auto bRet = GetMessage(&message, NULL, 0, 0);
        switch (bRet) {
        case -1:
        {
            auto flags = 0u
                | FORMAT_MESSAGE_ALLOCATE_BUFFER
                | FORMAT_MESSAGE_FROM_SYSTEM
                | FORMAT_MESSAGE_IGNORE_INSERTS;

            auto error = GetLastError();
            auto buffer = (WCHAR*)nullptr;
            FormatMessage(flags, NULL, error, 0u, (LPWSTR)&buffer, 0u, nullptr);
            OutputDebugString(buffer);

            exitCode = -1;
            return;
        }
        case 0:
        {
            exitCode = (int)message.wParam;
            return;
        }
        default:
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        }
    }
}

int MessageLoop::getExitCode() const {
    return exitCode;
}
