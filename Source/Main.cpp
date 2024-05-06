#ifndef UNICODE
    #define UNICODE
#endif 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "App.h"

int WINAPI wWinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow
)
{
    App app;

    if (app.Init(hInstance))
    {
        app.Run();
    }

    return 0;
}