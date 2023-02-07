#pragma once

#include <Windows.h>

namespace luaUi {
    class MessageLoop {
    public:
        MessageLoop();

        void run();
        int getExitCode() const;

    private:
        MSG message{};
        int exitCode{};
    };
}
