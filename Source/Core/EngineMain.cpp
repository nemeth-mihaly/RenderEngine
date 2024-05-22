#include "Common.h"
#include "Core/Engine.h"

int main()
{
    Engine engine;

    if (engine.Init())
        engine.Run();

    return 0;
}