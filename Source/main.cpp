#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "Application.h"

int main()
{
    Application app;
    if (app.Init())
    {
        app.Run();
    }

    return 0;
}