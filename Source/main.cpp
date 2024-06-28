#include <cstddef>
#include <cstdint>

#include "3rdParty/SDL2/SDL.h"

#include "Application.h"

int main(int argc, char* argv[])
{
    Application app;

    if (app.Init())
    {
        app.RunLoop();
    }

    return 0;
}