#include <cstddef>
#include <cstdint>

#include "3rdParty/SDL2/SDL.h"

#include "App.h"

int main(int argc, char* argv[])
{
    App app;

    if (app.Init())
        app.RunLoop();

    return 0;
}