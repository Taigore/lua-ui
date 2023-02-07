#include "MessageLoop.h"
#include "WindowPainter.h"
#include "LuaPainter.h"

#include <Windows.h>

class AppWindow {
public:
    AppWindow(HINSTANCE instance, int cmdShow);

    void setup();
    void paint();
    void setPainter(luaUi::WindowPainter* newPainter);

private:
    HINSTANCE instance;
    HWND window;
    ATOM clsHandle;
    int cmdShow;
    luaUi::WindowPainter* painter;
};

AppWindow::AppWindow(HINSTANCE instance, int cmdShow) {
    this->instance = instance;
    this->clsHandle = {};
    this->window = {};
    this->cmdShow = cmdShow;
    this->painter = &luaUi::WindowPainter::defaultPainter;
}

void AppWindow::paint() {
    painter->paintBegin(window);
    painter->paint();
    painter->paintFinish();
}

LRESULT WINAPI wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
    {
        auto createStruct = (CREATESTRUCT*)lParam;
        auto appWindow = (AppWindow*)createStruct->lpCreateParams;

        SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)appWindow);

        return 0;
    }
    case WM_PAINT:
    {
        auto appWindow = (AppWindow*)GetWindowLongPtr(window, GWLP_USERDATA);
        appWindow->paint();

        ValidateRect(window, nullptr);

        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(window, msg, wParam, lParam);
    }
}

void AppWindow::setup() {
    auto wndClass = WNDCLASSEX{};
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.lpszClassName = L"Main";
    wndClass.hInstance = instance;
    wndClass.lpfnWndProc = wndProc;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

    clsHandle = RegisterClassExW(&wndClass);

    window = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        (LPCWSTR)clsHandle,
        L"Main window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        instance,
        this);

    ShowWindow(window, cmdShow);
    UpdateWindow(window);
}

void AppWindow::setPainter(luaUi::WindowPainter* newPainter) {
    this->painter = newPainter;
}

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int cmdShow) {
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(cmdShow);

    auto painter = new luaUi::LuaPainter();
    painter->setup();

    auto window = new AppWindow{ instance, cmdShow };
    window->setPainter(painter);
    window->setup();

    auto loop = luaUi::MessageLoop{};
    loop.run();

    return loop.getExitCode();
}
