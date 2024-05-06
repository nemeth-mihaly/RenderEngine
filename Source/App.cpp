#include "App.h"

const wchar_t MyWindowClassName[] = TEXT("MyWindow");

// https://devblogs.microsoft.com/oldnewthing/20191014-00/?p=102992
//  ˘˘˘ 
LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    App* pApp = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pApp = static_cast<App*>(lpcs->lpCreateParams);
        SetWindowLongPtr(
            hWindow, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp)
        );

        pApp->m_hWindow = hWindow;
    }
    else
    {
        pApp = reinterpret_cast<App*>(
            GetWindowLongPtr(hWindow, GWLP_USERDATA)
        );
    }

    if (pApp)
    {
        return pApp->ProcMsg(uMsg, wParam, lParam);
    }

    return DefWindowProc(hWindow, uMsg, wParam, lParam);
}

App::App()
{
    m_hInstance = nullptr;
    m_hWindow = nullptr;
}

App::~App()
{
}

bool App::Init(HINSTANCE hInstance)
{
    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = MyWindowClassName;

    RegisterClass(&wc);

    m_hWindow = CreateWindowEx(
        0,
        MyWindowClassName,
        TEXT("RenderEngine"),
        WS_OVERLAPPEDWINDOW,
    
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    
        nullptr,
        nullptr,
        hInstance,
        this
    );

    if (m_hWindow == nullptr) 
    {
    }

    ShowWindow(m_hWindow, SW_SHOW);

    return true;
}

void App::Run()
{
    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Update and draw frame.
        }
    }
}

LRESULT App::ProcMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(m_hWindow, uMsg, wParam, lParam);
}