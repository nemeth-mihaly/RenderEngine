#include "Renderer.h"

#include <stdio.h>
#include <iostream>

#include "Application.h"

//-----------------------------------------------------------------------------
// Line3DRenderer Implementation
//-----------------------------------------------------------------------------

Line3DRenderer::Line3DRenderer()
{

}

void Line3DRenderer::Init()
{

}

void Line3DRenderer::AddLine3D(const glm::vec3& from, const glm::vec3& to)
{

}

void Line3DRenderer::Render(Shader* pShader)
{

}

//-----------------------------------------------------------------------------
// Box3DRenderer Implementation
//-----------------------------------------------------------------------------

Box3DRenderer::Box3DRenderer()
{
    m_numVerts = 0;
}

void Box3DRenderer::Init()
{
    m_numVerts = 24;

    std::vector<Vertex_UnlitColored> verts;
    verts.resize(m_numVerts);

    verts[0].pos = glm::vec3(-0.5f, 0.5f,-0.5f); verts[0].color = glm::vec3(1, 0, 1);
    verts[1].pos = glm::vec3( 0.5f, 0.5f,-0.5f); verts[1].color = glm::vec3(1, 0, 1);
    verts[2].pos = glm::vec3(-0.5f, 0.5f,-0.5f); verts[2].color = glm::vec3(1, 0, 1);
    verts[3].pos = glm::vec3(-0.5f,-0.5f,-0.5f); verts[3].color = glm::vec3(1, 0, 1);
    verts[4].pos = glm::vec3(-0.5f,-0.5f,-0.5f); verts[4].color = glm::vec3(1, 0, 1);
    verts[5].pos = glm::vec3( 0.5f,-0.5f,-0.5f); verts[5].color = glm::vec3(1, 0, 1);
    verts[6].pos = glm::vec3( 0.5f, 0.5f,-0.5f); verts[6].color = glm::vec3(1, 0, 1);
    verts[7].pos = glm::vec3( 0.5f,-0.5f,-0.5f); verts[7].color = glm::vec3(1, 0, 1);

    verts[8].pos = glm::vec3(-0.5f, 0.5f, 0.5f); verts[8].color = glm::vec3(1, 0, 1);
    verts[9].pos = glm::vec3( 0.5f, 0.5f, 0.5f); verts[9].color = glm::vec3(1, 0, 1);
    verts[10].pos = glm::vec3(-0.5f, 0.5f, 0.5f); verts[10].color = glm::vec3(1, 0, 1);
    verts[11].pos = glm::vec3(-0.5f,-0.5f, 0.5f); verts[11].color = glm::vec3(1, 0, 1);
    verts[12].pos = glm::vec3(-0.5f,-0.5f, 0.5f); verts[12].color = glm::vec3(1, 0, 1);
    verts[13].pos = glm::vec3( 0.5f,-0.5f, 0.5f); verts[13].color = glm::vec3(1, 0, 1);
    verts[14].pos = glm::vec3( 0.5f, 0.5f, 0.5f); verts[14].color = glm::vec3(1, 0, 1);
    verts[15].pos = glm::vec3( 0.5f,-0.5f, 0.5f); verts[15].color = glm::vec3(1, 0, 1);

    verts[16].pos = glm::vec3(-0.5f, 0.5f,-0.5f); verts[16].color = glm::vec3(1, 0, 1);
    verts[17].pos = glm::vec3(-0.5f, 0.5f, 0.5f); verts[17].color = glm::vec3(1, 0, 1);
    verts[18].pos = glm::vec3( 0.5f, 0.5f,-0.5f); verts[18].color = glm::vec3(1, 0, 1);
    verts[19].pos = glm::vec3( 0.5f, 0.5f, 0.5f); verts[19].color = glm::vec3(1, 0, 1);
    verts[20].pos = glm::vec3(-0.5f,-0.5f,-0.5f); verts[20].color = glm::vec3(1, 0, 1);
    verts[21].pos = glm::vec3(-0.5f,-0.5f, 0.5f); verts[21].color = glm::vec3(1, 0, 1);
    verts[22].pos = glm::vec3( 0.5f,-0.5f,-0.5f); verts[22].color = glm::vec3(1, 0, 1);
    verts[23].pos = glm::vec3( 0.5f,-0.5f, 0.5f); verts[23].color = glm::vec3(1, 0, 1);

    m_vertexArray.Init();
    m_vertexArray.SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_vertexArray.SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);

    long long vertBufferSize = sizeof(Vertex_UnlitColored) * verts.size();

    m_vertexBuffer.Init(vertBufferSize, GL_STATIC_DRAW);
    m_vertexBuffer.MapMemory(0, vertBufferSize, verts.data());

    m_vertexArray.SetVertexBuffer(0, &m_vertexBuffer, sizeof(Vertex_UnlitColored), VertexArrayInputRate_Vertex);
    
    m_vertexArray.SetVertexAttribute(1, 2, 4, GL_FLOAT, 0);
    m_vertexArray.SetVertexAttribute(1, 3, 4, GL_FLOAT, 16);
    m_vertexArray.SetVertexAttribute(1, 4, 4, GL_FLOAT, 32);
    m_vertexArray.SetVertexAttribute(1, 5, 4, GL_FLOAT, 48);

    m_instVertexBuffer.Init(sizeof(glm::mat4) * 64, GL_DYNAMIC_DRAW);
    m_vertexArray.SetVertexBuffer(1, &m_instVertexBuffer, sizeof(glm::mat4), VertexArrayInputRate_Instance);

    verts.clear();
}

void Box3DRenderer::AddBox3D(const glm::vec3& position, const glm::vec3& size)
{
    glm::mat4 transform = glm::translate(glm::mat4(1), position) * glm::scale(glm::mat4(1), size);

    m_instStagingBuffer.push_back(transform);
}

void Box3DRenderer::Render(Shader* pShader)
{
    pShader->Bind();
    pShader->SetUniformMatrix4f("uView", g_pApp->GetRenderer()->GetCamera()->GetView());
    pShader->SetUniformMatrix4f("uProjection", g_pApp->GetRenderer()->GetCamera()->GetProjection());

    m_instVertexBuffer.MapMemory(0, sizeof(glm::mat4) * m_instStagingBuffer.size(), m_instStagingBuffer.data());
    m_vertexArray.Bind();

    glDrawArraysInstanced(GL_LINES, 0, m_numVerts, m_instStagingBuffer.size());

    m_instStagingBuffer.clear();
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

    m_line3DRenderer.Init();
    m_box3DRenderer.Init();
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

    m_line3DRenderer.Render(&m_shader_UnlitColoredInstanced);
    m_box3DRenderer.Render(&m_shader_UnlitColoredInstanced);
}

void Renderer::AddLine3D(const glm::vec3& from, const glm::vec3& to)
{
    m_line3DRenderer.AddLine3D(from, to);
}

void Renderer::AddBox3D(const glm::vec3& position, const glm::vec3& size)
{
    m_box3DRenderer.AddBox3D(position, size);
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

void Renderer::OnResize(int width, int height)
{
    glViewport(0, 0, width, height);

    m_camera->OnResize(width, height);
}