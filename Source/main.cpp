#include "Common.h"
#include "Application.h"

int main()
{
    Application app;

    if (app.Init())
        app.Run();

    return 0;
}