#include "Renderer.h"

#include <stdio.h>
#include <iostream>

#include "Application.h"

//-----------------------------------------------------------------------------
// LineRenderer Implementation
//-----------------------------------------------------------------------------

LineRenderer::LineRenderer()
{
    m_bDepthEnabled = true;
}

void LineRenderer::Init()
{
    m_vertexArray.Init();
    m_vertexArray.SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_vertexArray.SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);

    long long vertBufferSize = sizeof(Vertex_UnlitColored) * 2024;

    m_vertexBuffer.Init(vertBufferSize, GL_DYNAMIC_DRAW);
    m_vertexArray.SetVertexBuffer(0, &m_vertexBuffer, sizeof(Vertex_UnlitColored), VertexArrayInputRate_Vertex);
}

struct Line 
{
    glm::vec3   fromPosition;
    glm::vec3   toPosition;

    Line(const glm::vec3& fromPosition_, const glm::vec3& toPosition_)
    {
        fromPosition = fromPosition_;
        toPosition = toPosition_;
    }
};

void LineRenderer::AddLine(const glm::vec3& fromPosition, const glm::vec3& toPosition, const glm::vec3& color)
{
    Vertex_UnlitColored v1;
    v1.pos = fromPosition;
    v1.color = color;

    Vertex_UnlitColored v2;
    v2.pos = toPosition;
    v2.color = color;

    m_stagingBuffer.push_back(v1);
    m_stagingBuffer.push_back(v2);
}

void LineRenderer::AddCross(const glm::vec3& position, float size, const glm::vec3& color)
{
    std::vector<Line> lines;

    lines.push_back(Line(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)));
    lines.push_back(Line(glm::vec3( 0,-1, 0), glm::vec3(0, 1, 0)));
    lines.push_back(Line(glm::vec3( 0, 0,-1), glm::vec3(0, 0, 1)));

    for (Line& line : lines)
    {
        line.fromPosition *= (size * 0.5f);
        line.fromPosition += position;

        line.toPosition *= (size * 0.5f);
        line.toPosition += position;

        AddLine(line.fromPosition, line.toPosition, color);
    }
}

void LineRenderer::AddBox(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color)
{
    std::vector<Line> lines;
    lines.reserve(12);

    lines.push_back(Line(glm::vec3(-1, 1,-1), glm::vec3( 1, 1,-1)));
    lines.push_back(Line(glm::vec3(-1, 1,-1), glm::vec3(-1,-1,-1)));
    lines.push_back(Line(glm::vec3(-1,-1,-1), glm::vec3( 1,-1,-1)));
    lines.push_back(Line(glm::vec3( 1, 1,-1), glm::vec3( 1,-1,-1)));

    lines.push_back(Line(glm::vec3(-1, 1, 1), glm::vec3( 1, 1, 1)));
    lines.push_back(Line(glm::vec3(-1, 1, 1), glm::vec3(-1,-1, 1)));
    lines.push_back(Line(glm::vec3(-1,-1, 1), glm::vec3( 1,-1, 1)));
    lines.push_back(Line(glm::vec3( 1, 1, 1), glm::vec3( 1,-1, 1)));

    lines.push_back(Line(glm::vec3(-1, 1,-1), glm::vec3(-1, 1, 1)));
    lines.push_back(Line(glm::vec3( 1, 1,-1), glm::vec3( 1, 1, 1)));

    lines.push_back(Line(glm::vec3(-1,-1,-1), glm::vec3(-1,-1, 1)));
    lines.push_back(Line(glm::vec3( 1,-1,-1), glm::vec3( 1,-1, 1)));

    for (Line& line : lines)
    {
        line.fromPosition *= (size * 0.5f);
        line.fromPosition += position;

        line.toPosition *= (size * 0.5f);
        line.toPosition += position;

        AddLine(line.fromPosition, line.toPosition, color);
    }
}

void LineRenderer::Render(Shader* pShader)
{
    if (!m_bDepthEnabled)
    {
        glDisable(GL_DEPTH_TEST);
    }

    pShader->Bind();
    pShader->SetUniformMatrix4f("uView", g_pApp->GetRenderer()->GetCamera()->GetView());
    pShader->SetUniformMatrix4f("uProjection", g_pApp->GetRenderer()->GetCamera()->GetProjection());
    
    pShader->SetUniformMatrix4f("uModel", glm::mat4(1));

    m_vertexBuffer.MapMemory(0, sizeof(Vertex_UnlitColored) * m_stagingBuffer.size(), m_stagingBuffer.data());
    m_vertexArray.Bind();

    glDrawArrays(GL_LINES, 0, m_stagingBuffer.size());

    m_stagingBuffer.clear();

    if (!m_bDepthEnabled)
    {
        glEnable(GL_DEPTH_TEST);
    }
}

void LineRenderer::ToggleDepthTest()
{
    m_bDepthEnabled = !m_bDepthEnabled;
}

//-----------------------------------------------------------------------------
// Renderer Implementation
//-----------------------------------------------------------------------------

Renderer::Renderer()
{
    m_sceneGraphRoot = nullptr;
    m_camera = nullptr;
}

Renderer::~Renderer()
{
    
}   

void Renderer::Init()
{
    const glm::ivec2 viewportSize = g_pApp->GetWindowSize(); // The viewport size always the same as the window size.

    glViewport(0, 0, viewportSize.x, viewportSize.y);

    glEnable(GL_DEPTH_TEST);

    m_shader_UnlitColored.Load("Assets/Shaders/UnlitColored1.vert", "Assets/Shaders/UnlitColored1.frag");
    m_shader_UnlitColoredInstanced.Load("Assets/Shaders/UnlitColoredInstanced.vert", "Assets/Shaders/UnlitColoredInstanced.frag");
    m_shader_UnlitTexturedColored.Load("Assets/Shaders/UnlitTexturedColored1.vert", "Assets/Shaders/UnlitTexturedColored1.frag");
    m_shader_UnlitTexturedColored2.Load("Assets/Shaders/UnlitTexturedColored2.vert", "Assets/Shaders/UnlitTexturedColored2.frag");
    m_shader_LitTexturedColored.Load("Assets/Shaders/LitTexturedColored1.vert", "Assets/Shaders/LitTexturedColored1.frag");

    m_sceneGraphRoot = std::make_shared<PawnNode>();

    m_camera = std::make_shared<Camera>();
    m_camera->Init();

    m_lineRenderer.Init();
}

void Renderer::Update(const float deltaTime)
{
    m_sceneGraphRoot->Update(deltaTime);

    m_camera->UpdateViewTransform();
}

void Renderer::Render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader_LitTexturedColored.Bind();
    m_shader_LitTexturedColored.SetUniformMatrix4f("uView", m_camera->GetView());
    m_shader_LitTexturedColored.SetUniformMatrix4f("uProjection", m_camera->GetProjection());

    m_shader_LitTexturedColored.SetUniform3f("uViewPosition", m_camera->GetPosition());

    m_shader_LitTexturedColored.SetUniform1i("uNumLights", m_lights.size());

    for (uint32_t i = 0; i < m_lights.size(); i++)
    {
        const std::string indexAsString = std::to_string(i);

        const Material& material = m_lights[i]->m_material;
        const LightProperties& lightProperties = m_lights[i]->GetProperties();

        m_shader_LitTexturedColored.SetUniform1i(("uLights[" + indexAsString + "].type"), lightProperties.type);
        m_shader_LitTexturedColored.SetUniform3f(("uLights[" + indexAsString + "].position"), lightProperties.position);
        m_shader_LitTexturedColored.SetUniform3f(("uLights[" + indexAsString + "].direction"), lightProperties.direction);
        m_shader_LitTexturedColored.SetUniform4f(("uLights[" + indexAsString + "].ambient"), material.ambient);
        m_shader_LitTexturedColored.SetUniform4f(("uLights[" + indexAsString + "].diffuse"), material.diffuse);
        m_shader_LitTexturedColored.SetUniform4f(("uLights[" + indexAsString + "].specular"), material.specular);
        m_shader_LitTexturedColored.SetUniform1f(("uLights[" + indexAsString + "].fallOff"), lightProperties.fallOff);
        m_shader_LitTexturedColored.SetUniform1f(("uLights[" + indexAsString + "].constantAttenuation"), lightProperties.constantAttenuation);
        m_shader_LitTexturedColored.SetUniform1f(("uLights[" + indexAsString + "].linearAttenuation"), lightProperties.linearAttenuation);
        m_shader_LitTexturedColored.SetUniform1f(("uLights[" + indexAsString + "].quadraticAttenuation"), lightProperties.quadraticAttenuation);
    }

    m_sceneGraphRoot->Render();
    m_sceneGraphRoot->RenderChildren();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < m_transparentSceneNodes.size(); i++)
    {
        for (int j = i + 1; j < m_transparentSceneNodes.size(); j++)
        {
            if (glm::length2(m_camera->GetPosition() - m_transparentSceneNodes[i]->GetPosition()) 
                > glm::length2(m_camera->GetPosition() - m_transparentSceneNodes[j]->GetPosition()))
            {
                std::shared_ptr<SceneNode> temp = m_transparentSceneNodes[i];
                m_transparentSceneNodes[i] = m_transparentSceneNodes[j];
                m_transparentSceneNodes[j] = temp;
            }
        }
    }

    for (int i = m_transparentSceneNodes.size() - 1; i >= 0; i--)
    {
        m_transparentSceneNodes[i]->Render();
    }

    m_transparentSceneNodes.clear();
    assert(m_transparentSceneNodes.empty());

    glDisable(GL_BLEND);

    m_lineRenderer.Render(&m_shader_UnlitColored);
}

void Renderer::AddLine(const glm::vec3& fromPosition, const glm::vec3& toPosition, const glm::vec3& color)
{
    m_lineRenderer.AddLine(fromPosition, toPosition, color);
}

void Renderer::AddCross(const glm::vec3& position, float size, const glm::vec3& color)
{
    m_lineRenderer.AddCross(position, size, color);
}

void Renderer::AddBox(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color)
{
    m_lineRenderer.AddBox(position, size, color);
}

void Renderer::AddLight(std::shared_ptr<SceneNode> node)
{
    std::shared_ptr<LightNode> light = std::dynamic_pointer_cast<LightNode>(node);
    m_lights.push_back(light);
}

void Renderer::RemoveLight(std::shared_ptr<SceneNode> node)
{
    std::shared_ptr<LightNode> light = std::dynamic_pointer_cast<LightNode>(node);

    for (std::vector<std::shared_ptr<LightNode>>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
    {
        if ((*it) != light)
        {
            continue;
        }

        m_lights.erase(it);

        break;
    }
}

void Renderer::AddTransparentSceneNode(std::shared_ptr<SceneNode> node)
{
    m_transparentSceneNodes.push_back(node);
}

void Renderer::OnResize(int width, int height)
{
    glViewport(0, 0, width, height);

    m_camera->OnResize(width, height);
}