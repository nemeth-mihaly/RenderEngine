#include "Common.h"
#include "Core/Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Application app(hInstance);
    
    if (app.Initialize())
        app.Run();

    return 0;
}
