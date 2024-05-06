#ifndef UNICODE
    #define UNICODE
#endif 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Application.h"

int WINAPI wWinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow
)
{
    Application app(hInstance);
    
    if (app.Init())
    {
        app.MainLoop();
    }

    return 0;
}