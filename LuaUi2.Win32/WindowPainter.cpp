#include "WindowPainter.h"

#include <Windows.h>

using luaUi::WindowPainter;

WindowPainter WindowPainter::defaultPainter{};

WindowPainter::WindowPainter() {
    this->dc = {};
    this->paintStruct = {};
    this->window = {};
}

WindowPainter::~WindowPainter() = default;

void WindowPainter::paintBegin(HWND pWindow) {
    window = pWindow;
    dc = BeginPaint(pWindow, &paintStruct);
}

void WindowPainter::paint() {
    auto brush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(dc, &(paintStruct.rcPaint), brush);
    DeleteObject(brush);
}

void WindowPainter::paintFinish() {
    EndPaint(window, &paintStruct);
    dc = {};
    paintStruct = {};
    window = {};
}

HDC WindowPainter::getDc() const {
    return dc;
}
