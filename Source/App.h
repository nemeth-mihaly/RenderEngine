#pragma once

#ifndef UNICODE
    #define UNICODE
#endif 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class App
{
    friend LRESULT CALLBACK WindowProc(
        HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam
    );

 public:
    App();
    ~App();

    bool Init(HINSTANCE hInstance);
    void Run();

    LRESULT ProcMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

 private:
    HINSTANCE m_hInstance;
    HWND m_hWindow;
};