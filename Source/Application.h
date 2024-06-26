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
// struct Vertex_UnlitColored
//-----------------------------------------------------------------------------

struct Vertex_UnlitColored
{
    glm::vec3   pos;
};

//-----------------------------------------------------------------------------
// class Brush
//-----------------------------------------------------------------------------

/*
class Brush 
{
public:
    Brush()
    {
    }

    ~Brush()
    {
    }

    void Load()
    {
        m_vertices.resize(4);
        m_vertices[0].pos = glm::vec3(-0.5f, 0.0f, 0.5f);
        m_vertices[1].pos = glm::vec3( 0.5f, 0.0f, 0.5f);
        m_vertices[2].pos = glm::vec3(-0.5f, 0.0f,-0.5f);
        m_vertices[3].pos = glm::vec3( 0.5f, 0.0f,-0.5f);
    
        std::vector<uint8_t> indices =
        {
            0, 2, 1,
            1, 2, 3,
        };

        m_vertexArray.reset(new VertexArray());
        m_vertexArray->SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);

        const signed long long vertexBufferSize = sizeof(Vertex_UnlitColored) * m_vertices.size();
        m_vertexBuffer.reset(new VertexBuffer(vertexBufferSize, GL_STATIC_DRAW));
        m_vertexBuffer->MapMemory(0, vertexBufferSize, m_vertices.data());
        m_vertexArray->SetVertexBuffer(0, m_vertexBuffer, sizeof(Vertex_UnlitColored), VertexArrayInputRate_Vertex);

        const signed long long indexBufferSize = sizeof(uint32_t) * indices.size();
        m_indexBuffer.reset(new IndexBuffer(indexBufferSize, GL_STATIC_DRAW));
        m_indexBuffer->MapMemory(0, indexBufferSize, indices.data());
        m_vertexArray->SetIndexBuffer(m_indexBuffer);
        m_indexCount = indices.size();
        indices.clear();
    }

    void Update(const float deltaTime)
    {
    }

    void Render()
    {
        g_pShader_UnlitColored->Bind();

        m_vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_BYTE, nullptr);
    }

private:
    std::vector<Vertex_UnlitColored>     m_vertices;

    uint32_t                m_indexCount;

    StrongVertexArrayPtr    m_vertexArray;
    StrongVertexBufferPtr   m_vertexBuffer;
    StrongIndexBufferPtr    m_indexBuffer;
};
*/

//-----------------------------------------------------------------------------
// class Application
//-----------------------------------------------------------------------------

class Application
{
public:
    Application();
    ~Application();

    bool Initialize();
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
    SDL_GLContext   m_Context;

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