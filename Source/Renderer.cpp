#include "Renderer.h"

#include <stdio.h>
#include <iostream>

#include "Application.h"

bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& sphereCenter, float sphereRadius)
{
    glm::vec3 L = sphereCenter - rayOrigin;
    float tc = glm::dot(L, rayDir);

    if (tc < 0.0f)
    {
        return false;
    }

    float d2 = glm::dot(L, L) - tc * tc;
    
    if (d2 > sphereRadius * sphereRadius)
    {
        return false;
    }

    return true;
}

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

    long long vertBufferSize = sizeof(Vertex_UnlitColored) * GetMaxVerts();

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

void LineRenderer::AddCircle(const glm::vec3& position, const glm::vec3& normal, float radius, const glm::vec3& color)
{
    int numSegments = 32;

    std::vector<glm::vec3> verts;
    verts.reserve(numSegments);

    for (int i = 0; i < numSegments; i++)
    {
        float theta = ((2.0f * glm::pi<float>()) / static_cast<float>(numSegments)) * static_cast<float>(i);

        glm::vec3 vertex;
        vertex.x = cosf(theta);
        vertex.y = 0.0f;
        vertex.z = sinf(theta);

        vertex *= radius;
        vertex += position;

        verts.push_back(vertex);
    }    

    for (int i = 0; i < numSegments; i++) 
    {
        int pv1 = i;
        int pv2 = (i + 1) % numSegments;

        glm::vec3 v1 = verts[pv1];
        glm::vec3 v2 = verts[pv2];
        AddLine(v1, v2, color);
    }   

    verts.clear();
}

void LineRenderer::AddSphere(const glm::vec3& position, float radius, const glm::vec3& color)
{
    glm::ivec2 numSegments(12, 12);

    std::vector<glm::vec3> verts;
    verts.reserve(numSegments.x * (numSegments.y + 1));

    for (int y = 0; y <= numSegments.y; y++)
    {
        float phi = (glm::pi<float>() / float_t(numSegments.y)) * y;
        phi -= 0.5f * glm::pi<float>();

        for (int x = 0; x < numSegments.x; x++)
        {
            float theta = ((2.0f * glm::pi<float>()) / static_cast<float>(numSegments.x)) * static_cast<float>(x);

            glm::vec3 vertex;
            vertex.x = cosf(phi) * cosf(theta);
            vertex.y = sinf(phi);
            vertex.z = cosf(phi) * sinf(theta);

            vertex *= radius;
            vertex += position;

            verts.push_back(vertex);
        }
    }

    for (int y = 0; y <= numSegments.y; y++) 
    {
        for (int x = 0; x < numSegments.x; x++)
        {
            int pv1 = x + y * numSegments.x;
            int pv2 = ((x + 1) % numSegments.x) + y * numSegments.x;

            glm::vec3 v1 = verts[pv1];
            glm::vec3 v2 = verts[pv2];
            AddLine(v1, v2, color);

            if (!(y < numSegments.y))
            {
                continue;
            }

            int pv3 = x + (y + 1) * numSegments.x;
            glm::vec3 v3 = verts[pv3];
            AddLine(v1, v3, color);
        }
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

    m_numVertsLastFrame = m_stagingBuffer.size();
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
    m_shader_UnlitColoredTexturedTerrain.Load("Assets/Shaders/UnlitTexturedColoredTerrain.vert", "Assets/Shaders/UnlitTexturedColoredTerrain.frag");
    m_shader_Framebuffer.Load("Assets/Shaders/_test_Framebuffer.vert", "Assets/Shaders/_test_Framebuffer.frag");

    m_framebuffer.Init();

    m_sceneGraphRoot = std::make_shared<PawnNode>();

    m_lineRenderer.Init();

    std::vector<Vertex_UnlitTexturedColored> verts;
    verts.resize(6);

    verts[0].pos = glm::vec3(-1, 1, 0); verts[0].color = glm::vec3(1, 1, 1); verts[0].uv = glm::vec2(0, 1);
    verts[1].pos = glm::vec3( 1, 1, 0); verts[1].color = glm::vec3(1, 1, 1); verts[1].uv = glm::vec2(1, 1);
    verts[2].pos = glm::vec3(-1,-1, 0); verts[2].color = glm::vec3(1, 1, 1); verts[2].uv = glm::vec2(0, 0);

    verts[3].pos = glm::vec3(-1,-1, 0); verts[3].color = glm::vec3(1, 1, 1); verts[3].uv = glm::vec2(0, 0);
    verts[4].pos = glm::vec3( 1,-1, 0); verts[4].color = glm::vec3(1, 1, 1); verts[4].uv = glm::vec2(1, 0);
    verts[5].pos = glm::vec3( 1, 1, 0); verts[5].color = glm::vec3(1, 1, 1); verts[5].uv = glm::vec2(1, 1);

    m_fboVertArray.Init();
    m_fboVertArray.SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_fboVertArray.SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_fboVertArray.SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);

    GLsizeiptr fboVertBufferSize = sizeof(Vertex_UnlitTexturedColored) * verts.size();
    m_fboVertBuffer.Init(fboVertBufferSize, GL_STATIC_DRAW);
    m_fboVertBuffer.MapMemory(0, fboVertBufferSize, verts.data());

    m_fboVertArray.SetVertexBuffer(0, &m_fboVertBuffer, sizeof(Vertex_UnlitTexturedColored), VertexArrayInputRate_Vertex);

    m_fboNumVerts = verts.size();
}

void Renderer::Update(const float deltaTime)
{
    m_sceneGraphRoot->Update(deltaTime);
}

void Renderer::Render()
{
    m_framebuffer.Bind();

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_world->Render();

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

    /*
    glm::vec3 position;
    if (FindIntersectionPoint(position))
    {
        // std::cout << "p (" << position.x << ' ' << position.z << ")\n";
        AddCross(position, 0.4f, glm::vec3(1, 0, 0));
        
        float radius = 1.0f;
        m_lineRenderer.AddCircle(position, glm::vec3(0, 1, 0), radius, glm::vec3(1, 1, 1));

        std::vector<Vertex_UnlitColored*> verts;

        glm::ivec3 min;
        min.x = glm::max(0, (int)glm::ceil(position.x - radius));
        min.z = glm::max(0, (int)glm::ceil(position.z - radius));

        glm::ivec3 max;
        max.x = glm::min((m_extents.x - 1), (int)glm::floor(position.x + radius));
        max.z = glm::min((m_extents.z - 1), (int)glm::floor(position.z + radius));

        for (int z = min.z; z <= max.z; z++)
        {
            for (int x = min.x; x <= max.x; x++)
            {
                //  verts.push_back(&m_verts[x + z * m_extents.z]);
            }       
        }
    }
    */

    m_lineRenderer.Render(&m_shader_UnlitColored);

    m_framebuffer.Unbind();

    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader_Framebuffer.Bind();
    m_framebuffer.BindTexture();

    m_fboVertArray.Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_fboNumVerts);

    m_framebuffer.UnbindTexture();
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

void Renderer::AddCircle(const glm::vec3& position, const glm::vec3& normal, float radius, const glm::vec3& color)
{
    m_lineRenderer.AddCircle(position, normal, radius, color);
}

void Renderer::AddSphere(const glm::vec3& position, float radius, const glm::vec3& color)
{
    m_lineRenderer.AddSphere(position, radius, color);
}

void Renderer::SetCamera(std::shared_ptr<Camera> camera)
{
    if (m_camera)
    {
        m_camera->m_bActive = false;
    }
    
    m_camera = camera;
    m_camera->m_bActive = true;
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