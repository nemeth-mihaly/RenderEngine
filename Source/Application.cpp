#include "Application.h"

Application* g_pApp = nullptr;

//-----------------------------------------------------------------------------
// Application Implementation
//-----------------------------------------------------------------------------

Application::Application()
{
    g_pApp = this;
}

Application::~Application()
{
    
}

bool Application::Init()
{
    return true;
}

void Application::Run()
{

}