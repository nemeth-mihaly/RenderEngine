#include <stddef.h>
#include <stdint.h>

#include "Application.h"

int main(int argc, char* argv[])
{
    Application app;

    if (app.Init(1280, 720))
    {
        app.RunLoop();
    }

    return 0;
}