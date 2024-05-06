#pragma once

#ifndef UNICODE
    #define UNICODE
#endif 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Win32 programming in C++:
//  https://learn.microsoft.com/en-us/windows/win32/learnwin32/your-first-windows-program

class Application
{
    friend LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam);

 public:
    Application(HINSTANCE hInstance);
    ~Application();

    bool Init();
    void MainLoop();

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

 private:
    HINSTANCE m_hInstance;
    HWND m_hWindow;
    
    HDC m_hDeviceContext;
    HGLRC m_hGLRenderContext;
};