#include <Windows.h>

class AppWindow {
public:
    AppWindow(HINSTANCE instance, int cmdShow);

    void setup();

private:
    HINSTANCE instance;
    HWND winHandle;
    ATOM clsHandle;
    int cmdShow;
};

AppWindow::AppWindow(HINSTANCE instance, int cmdShow) {
    this->instance = instance;
    this->clsHandle = {};
    this->winHandle = {};
    this->cmdShow = cmdShow;
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

    winHandle = CreateWindowEx(
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

    ShowWindow(winHandle, cmdShow);
    UpdateWindow(winHandle);
}

class MessageLoop {
public:
    void run();
    int getExitCode() const;

private:
    MSG message;
    int exitCode;
    BYTE padding[4];
};

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

int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int cmdShow) {
    UNREFERENCED_PARAMETER(prevInstance);
    UNREFERENCED_PARAMETER(cmdLine);
    UNREFERENCED_PARAMETER(cmdShow);

    auto window = new AppWindow{ instance, cmdShow };
    window->setup();

    auto loop = MessageLoop{};
    loop.run();

    return loop.getExitCode();
}
