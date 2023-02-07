#pragma once

#include <Windows.h>

namespace luaUi {
    class WindowPainter {
    public:
        static WindowPainter defaultPainter;

        WindowPainter();
        virtual ~WindowPainter();

        void paintBegin(HWND pWindow);
        virtual void paint();
        void paintFinish();

    protected:
        HDC getDc() const;

    private:
        HWND window;
        HDC dc;
        PAINTSTRUCT paintStruct;
    };
}
