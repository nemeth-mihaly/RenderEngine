#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "3rdParty/SDL2/SDL.h"

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

#include "3rdParty/imgui/imgui.h"
#include "3rdParty/imgui/imgui_impl_sdl2.h"
#include "3rdParty/imgui/imgui_impl_opengl3.h"

#include "Scene.h"

//-----------------------------------------------------------------------------
// class PerformanceInfoControl
//-----------------------------------------------------------------------------

class PerformanceInfoControl
{
public:
    PerformanceInfoControl()
    {
        m_elapsedTimeInSeconds = 0;

        m_frameCount = 0;
        m_fps = 0;

        m_numShaderSwaps = 0;
        m_numShaderUniformLookups = 0;
    }

    ~PerformanceInfoControl() 
    {
    }

    void Update(const float deltaTime) 
    {
        m_elapsedTimeInSeconds += deltaTime;
        m_frameCount++;

        if (m_elapsedTimeInSeconds >= 1.0f)
        {
            m_elapsedTimeInSeconds = 0.0f;

            m_fps = m_frameCount;
            m_frameCount = 0;
        
            m_numShaderSwaps = g_numShaderSwaps / m_fps;
            m_numShaderUniformLookups = g_numUniformLocationLookups / m_fps;
            g_numShaderSwaps = 0;
            g_numUniformLocationLookups = 0;
        }
    }

    void Render() 
    {
        ImGui::Begin("Performance Info");
        ImGui::Text("Application average %.3f ms/frame (%i FPS)", 1000.0f / (float)m_fps, m_fps);
        ImGui::Text("Num shader swaps: %d", m_numShaderSwaps);
        ImGui::Text("Num shader uniform lookups: %d", m_numShaderUniformLookups);
        ImGui::End();
    }

private:
    float   m_elapsedTimeInSeconds;

    int     m_frameCount;
    int     m_fps;

    int m_numShaderSwaps;
    int m_numShaderUniformLookups;
};

//-----------------------------------------------------------------------------
// class Application
//-----------------------------------------------------------------------------

class Application
{
public:
    Application();
    ~Application();

    bool Init();
    void RunLoop();    

private:
    void ProcessMessages();
    bool OnMsgProc(const SDL_Event& event);
    bool OnImGUIMsgProc(const SDL_Event& event);

    bool OnKeyDown(int key);
    bool OnKeyUp(int key);
    bool OnMouseMove(const glm::ivec2& pos);
    bool OnMouseButtonDown(int button);
    bool OnMouseButtonUp(int button);

    void Update(const float deltaTime);
    void UpdateMovementController(const float deltaTime);

    void Render();
    void ImGUIRender();

private:
    bool            m_bRunning;

    SDL_Window*     m_pWindow;
    SDL_GLContext   m_pGLContext;

    uint64_t        m_currentTime;
    uint64_t        m_lastTime;

    float           m_deltaTime;

    static const int MAX_KEYS = 512;
    bool            m_bKeys[MAX_KEYS];

    glm::vec2       m_currentMousePos;
    glm::vec2       m_lastMousePos;

    Scene*          m_pScene;

    bool            m_bDebugCameraEnabled = false;
    bool            m_bCameraMoving;
    float           m_Yaw;
    float           m_Pitch;

    PerformanceInfoControl m_performanceInfoControl;
    // Brush m_brush;
};

extern Application* g_pApp;