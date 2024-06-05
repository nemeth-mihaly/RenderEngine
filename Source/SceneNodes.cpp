#include "SceneNodes.h"

#include "Application.h"
#include "Scene.h"

////////////////////////////////////////////////////
//  SceneNode Implementation
////////////////////////////////////////////////////

SceneNode::SceneNode()
{
    m_Pos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    m_Rotation = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
}

SceneNode::~SceneNode()
{
}

void SceneNode::Update(Scene* pScene, const float deltaTime)
{
}

void SceneNode::Render(Scene* pScene)
{
}

////////////////////////////////////////////////////
//  CameraNode Implementation
////////////////////////////////////////////////////

CameraNode::CameraNode()
    : SceneNode()
{
    m_Pos = glm::vec3(0.0f, 0.0f, 3.0f);
    m_ForwardDir = glm::vec3(0.0f, 0.0f, -1.0f);
    m_Projection = glm::perspective(glm::radians(45.0f), (1280 / static_cast<float>(720)), 0.001f, 1000.0f);
    WorldViewProjection();
}

CameraNode::~CameraNode()
{
}

glm::mat4 CameraNode::WorldViewProjection()
{
    m_View = glm::lookAt(m_Pos, (m_Pos + m_ForwardDir), glm::vec3(0.0f, 1.0f, 0.0f));
    return (m_Projection * m_View);
}

////////////////////////////////////////////////////
//  MeshNode Implementation
////////////////////////////////////////////////////

MeshNode::MeshNode(const std::string& meshName, const std::string& shaderProgName, const std::string& textureName)
    : SceneNode(), m_MeshName(meshName), m_ShaderProgName(shaderProgName), m_TextureName(textureName)
{
}

MeshNode::~MeshNode()
{
}

void MeshNode::Render(Scene* pScene)
{
    g_TexturedLitShader->Bind();
    g_TexturedLitShader->SetUniform4f("u_Material.Ambient", Material.Ambient);
    g_TexturedLitShader->SetUniform4f("u_Material.Diffuse", Material.Diffuse);
    g_TexturedLitShader->SetUniform4f("u_Material.Specular", Material.Specular);
    g_TexturedLitShader->SetUniform4f("u_Material.Emissive", Material.Emissive);
    g_TexturedLitShader->SetUniform1f("u_Material.SpecularPower", Material.SpecularPower);
    g_TexturedLitShader->SetUniform1b("u_Material.bUseTexture", Material.bUseTexture);

    if (Material.bUseTexture)
    {
        const uint32_t textureUnit = 0;
        g_UvGridTexture->BindUnit(textureUnit);
        g_TexturedLitShader->SetUniform1i("u_Texture", textureUnit);
    }

    glm::mat4 world = glm::translate(glm::mat4(1.0f), m_Pos);
    world *= glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.w), glm::vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z));
    world *= glm::scale(glm::mat4(1.0f), m_Scale);

    g_TexturedLitShader->SetUniformMatrix4f("u_World", world);

    //StrongMeshPtr mesh = g_pApp->GetResourceManager().GetMesh(m_MeshName);

    g_MonkeyMesh->GetVertexArray()->Bind();
    glDrawArrays(GL_TRIANGLES, 0, g_MonkeyMesh->GetVertexCount());
}

////////////////////////////////////////////////////
//  SkyNode Implementation
////////////////////////////////////////////////////

const uint32_t GNumCubeSides = 6;
const uint32_t GNumSideVertices = 6;

SkyNode::SkyNode()
    : SceneNode()
{
    std::vector<Vertex> vertices =
    {
        /** POSITIVE_X */

        {{ 0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }},
        {{ 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ 0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},

        {{ 0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},
        {{ 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ 0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }},

        /** NEGATIVE_X */

        {{ -0.5f,  0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ -0.5f,  0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},

        {{ -0.5f,  0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ -0.5f, -0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }},

        /** POSITIVE_Y */

        {{  0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }},
        {{  0.5f, 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }},

        {{  0.5f, 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ -0.5f, 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }},

        /** NEGATIVE_Y */

        {{  0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }},
        {{  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }},

        {{  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ -0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }},

        /** POSITIVE_Z */

        {{ -0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
        {{  0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},

        {{  0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
        {{  0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }},

        /** NEGATIVE_Z */

        {{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f }},
        {{  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f }},

        {{  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f }},
        {{  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f }},
    };

    m_VertexBuffer.reset(new VertexBuffer(sizeof(Vertex) * vertices.size(), GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, sizeof(Vertex) * vertices.size(), vertices.data());

    m_VertexArray.reset(new VertexArray());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    m_VertexArray->SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_VertexArray->SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_VertexArray->SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);

    m_VertexCount = vertices.size();
    vertices.clear();

    for (uint32_t Index = 0; Index < GNumCubeSides; Index++)
    {
        m_Textures[Index].reset(new Texture(GL_TEXTURE_2D));
    }

    m_Textures[0]->LoadFromFile("Assets/Textures/Sky_PX_dbg.png");
    m_Textures[1]->LoadFromFile("Assets/Textures/Sky_NX_dbg.png");
    m_Textures[2]->LoadFromFile("Assets/Textures/Sky_PY_dbg.png");
    m_Textures[3]->LoadFromFile("Assets/Textures/Sky_NY_dbg.png");
    m_Textures[4]->LoadFromFile("Assets/Textures/Sky_PZ_dbg.png");
    m_Textures[5]->LoadFromFile("Assets/Textures/Sky_NZ_dbg.png");
}

SkyNode::~SkyNode()
{
}

void SkyNode::Render(Scene* pScene)
{
    glDepthFunc(GL_LEQUAL);

    g_SkyShader->Bind();
    g_SkyShader->SetUniformMatrix4f("u_WorldView", glm::mat4(glm::mat3(pScene->GetCamera()->GetView())));
    g_SkyShader->SetUniformMatrix4f("u_WorldProjection", pScene->GetCamera()->GetProjection());

    m_VertexArray->Bind();

    for (uint32_t Side = 0; Side < GNumCubeSides; Side++)
    {
        const uint32_t textureUnit = 0;
        m_Textures[Side]->BindUnit(textureUnit);
        g_SkyShader->SetUniform1i("u_Texture", textureUnit);

        const uint32_t VertexBufferOffset = Side * GNumSideVertices;
        glDrawArrays(GL_TRIANGLES, VertexBufferOffset, GNumSideVertices);
    }

    glDepthFunc(GL_LESS);
}

////////////////////////////////////////////////////
//  BillboardNode Implementation
////////////////////////////////////////////////////

BillboardNode::BillboardNode()
{
    std::vector<Vertex> vertices;
    vertices.resize(4);

    const float dim = 0.5f;

    vertices[0].Pos = glm::vec3(-dim, dim, 0.0f ); vertices[0].Normal = glm::vec3( 0.0f, 1.0f, 0.0f ); vertices[0].Uv = glm::vec2( 0.0f, 1.0f );
    vertices[1].Pos = glm::vec3( dim, dim, 0.0f ); vertices[1].Normal = glm::vec3( 0.0f, 1.0f, 0.0f ); vertices[1].Uv = glm::vec2( 1.0f, 1.0f );
    vertices[2].Pos = glm::vec3(-dim,-dim, 0.0f ); vertices[2].Normal = glm::vec3( 0.0f, 1.0f, 0.0f ); vertices[2].Uv = glm::vec2( 0.0f, 0.0f );
    vertices[3].Pos = glm::vec3( dim,-dim, 0.0f ); vertices[3].Normal = glm::vec3( 0.0f, 1.0f, 0.0f ); vertices[3].Uv = glm::vec2( 1.0f, 0.0f );

    m_VertexBuffer.reset(new VertexBuffer(sizeof(Vertex) * vertices.size(), GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, sizeof(Vertex) * vertices.size(), vertices.data());

    vertices.clear();

    std::vector<uint32_t> indices = 
    {
        0, 2, 1,
        1, 2, 3
    };

    m_IndexBuffer.reset(new IndexBuffer(sizeof(uint32_t) * indices.size(), GL_STATIC_DRAW));
    m_IndexBuffer->MapMemory(0, sizeof(uint32_t) * indices.size(), indices.data());

    m_IndexCount = indices.size();
    indices.clear();

    m_VertexArray.reset(new VertexArray());

    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    m_VertexArray->SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_VertexArray->SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_VertexArray->SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);
}

BillboardNode::~BillboardNode()
{
}

void BillboardNode::Render(Scene* pScene)
{
    g_BillboardShader->Bind();

    if (Material.bUseTexture)
    {
        const uint32_t textureUnit = 0;
        g_UvGridTexture->BindUnit(textureUnit);
        g_BillboardShader->SetUniform1i("u_Texture", textureUnit);
    }

    g_BillboardShader->SetUniform3f("u_Position", m_Pos);

    m_VertexArray->Bind();

    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}