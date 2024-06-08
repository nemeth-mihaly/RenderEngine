#include "SceneNodes.h"

#include "Application.h"
#include "Scene.h"

////////////////////////////////////////////////////
//  SceneNode Implementation
////////////////////////////////////////////////////

SceneNode::SceneNode()
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::Update(Scene* pScene, const float deltaTime)
{
    const glm::mat4 idmat = glm::mat4(1.0f);
    m_WorldTransform = glm::translate(idmat, m_Position);
}

void SceneNode::Render(Scene* pScene)
{
    return;
}

////////////////////////////////////////////////////
//  CameraNode Implementation
////////////////////////////////////////////////////

CameraNode::CameraNode()
{
    m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
    m_ForwardDir = glm::vec3(0.0f, 0.0f, -1.0f);
    m_TargetPos = m_Position + m_ForwardDir;
    m_Projection = glm::perspective(glm::radians(45.0f), (1280 / static_cast<float>(720)), 0.001f, 1000.0f);
    WorldViewProjection();
}

CameraNode::~CameraNode()
{
}

glm::mat4 CameraNode::WorldViewProjection()
{
    //m_View = glm::lookAt(m_Pos, (m_Pos + m_ForwardDir), glm::vec3(0.0f, 1.0f, 0.0f));
    m_View = glm::lookAt(m_Position, m_TargetPos, glm::vec3(0.0f, 1.0f, 0.0f));
    return (m_Projection * m_View);
}

////////////////////////////////////////////////////
//  MeshNode Implementation
////////////////////////////////////////////////////

MeshNode::MeshNode(const StrongMeshPtr& mesh, const StrongShaderPtr& shader, const StrongTexturePtr& texture)
    : m_Mesh(mesh), m_Shader(shader), m_Texture(texture)
{
}

MeshNode::~MeshNode()
{
}

void MeshNode::Render(Scene* pScene)
{
    m_Shader->Bind();

    m_Shader->SetUniformMatrix4f("u_World", m_WorldTransform);

    m_Shader->SetUniform4f("u_Material.Ambient", m_Material.Ambient);
    m_Shader->SetUniform4f("u_Material.Diffuse", m_Material.Diffuse);
    m_Shader->SetUniform4f("u_Material.Specular", m_Material.Specular);
    m_Shader->SetUniform4f("u_Material.Emissive", m_Material.Emissive);
    m_Shader->SetUniform1f("u_Material.SpecularPower", m_Material.SpecularPower);
    m_Shader->SetUniform1b("u_Material.bUseTexture", m_Material.bUseTexture);

    if (m_Material.bUseTexture)
    {
        const uint32_t textureUnit = 0;
        m_Texture->BindUnit(textureUnit);
        m_Shader->SetUniform1i("u_Texture", textureUnit);
    }

    m_Mesh->GetVertexArray()->Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_Mesh->GetVertexCount());
}

////////////////////////////////////////////////////
//  CubeMapNode Implementation
////////////////////////////////////////////////////

CubeMapNode::CubeMapNode()
{
    m_CubeMapSideVertCount = 6;

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

    m_VertexArray.reset(new VertexArray());
    m_VertexArray->SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_VertexArray->SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_VertexArray->SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);

    const ssize_t vertexBufferSize = sizeof(Vertex) * vertices.size();
    m_VertexBuffer.reset(new VertexBuffer(vertexBufferSize, GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, vertexBufferSize, vertices.data());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    m_VertCount = vertices.size();
    vertices.clear();

    for (uint32_t i = 0; i < CubeMapSide_Count; i++)
        m_Textures[i].reset(new Texture(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE, GL_LINEAR));

    m_Textures[CubeMapSide_E]->LoadResource("Assets/Textures/Sky_PX_dbg.png");
    m_Textures[CubeMapSide_W]->LoadResource("Assets/Textures/Sky_NX_dbg.png");
    m_Textures[CubeMapSide_U]->LoadResource("Assets/Textures/Sky_PY_dbg.png");
    m_Textures[CubeMapSide_D]->LoadResource("Assets/Textures/Sky_NY_dbg.png");
    m_Textures[CubeMapSide_N]->LoadResource("Assets/Textures/Sky_PZ_dbg.png");
    m_Textures[CubeMapSide_S]->LoadResource("Assets/Textures/Sky_NZ_dbg.png");
}

CubeMapNode::~CubeMapNode()
{
}

void CubeMapNode::Render(Scene* pScene)
{
    glDepthFunc(GL_LEQUAL);

    g_SkyShader->Bind();
    g_SkyShader->SetUniformMatrix4f("u_WorldView", glm::mat4(glm::mat3(pScene->GetCamera()->GetView())));
    g_SkyShader->SetUniformMatrix4f("u_WorldProjection", pScene->GetCamera()->GetProjection());

    m_VertexArray->Bind();

    for (uint32_t i = 0; i < CubeMapSide_Count; i++)
    {
        const uint32_t texUnit = 0;
        m_Textures[i]->BindUnit(texUnit);
        g_SkyShader->SetUniform1i("u_Texture", texUnit);

        const int vertFirst = i * m_CubeMapSideVertCount;
        glDrawArrays(GL_TRIANGLES, vertFirst, m_CubeMapSideVertCount);
    }

    glDepthFunc(GL_LESS);
}

////////////////////////////////////////////////////
//  BillboardNode Implementation
////////////////////////////////////////////////////

BillboardNode::BillboardNode(const StrongTexturePtr& texture)
    : m_Texture(texture)
{
    const uint32_t rectVertCount = 4;
    std::vector<Vertex> vertices;
    vertices.resize(rectVertCount);

    const float dim = 0.5f;

    vertices[0].Pos = glm::vec3(-dim, dim, 0.0f ); vertices[0].Normal = glm::vec3( 0.0f, 1.0f, 0.0f ); vertices[0].Uv = glm::vec2( 0.0f, 1.0f );
    vertices[1].Pos = glm::vec3( dim, dim, 0.0f ); vertices[1].Normal = glm::vec3( 0.0f, 1.0f, 0.0f ); vertices[1].Uv = glm::vec2( 1.0f, 1.0f );
    vertices[2].Pos = glm::vec3(-dim,-dim, 0.0f ); vertices[2].Normal = glm::vec3( 0.0f, 1.0f, 0.0f ); vertices[2].Uv = glm::vec2( 0.0f, 0.0f );
    vertices[3].Pos = glm::vec3( dim,-dim, 0.0f ); vertices[3].Normal = glm::vec3( 0.0f, 1.0f, 0.0f ); vertices[3].Uv = glm::vec2( 1.0f, 0.0f );

    std::vector<uint32_t> indices = 
    {
        0, 2, 1,
        1, 2, 3
    };

    m_VertexArray.reset(new VertexArray());
    m_VertexArray->SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_VertexArray->SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_VertexArray->SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);

    const ssize_t vertexBufferSize = sizeof(Vertex) * vertices.size();
    m_VertexBuffer.reset(new VertexBuffer(vertexBufferSize, GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, vertexBufferSize, vertices.data());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    vertices.clear();

    const ssize_t indexBufferSize = sizeof(uint32_t) * indices.size();
    m_IndexBuffer.reset(new IndexBuffer(indexBufferSize, GL_STATIC_DRAW));
    m_IndexBuffer->MapMemory(0, indexBufferSize, indices.data());
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    m_IndexCount = indices.size();
    indices.clear();
}

BillboardNode::~BillboardNode()
{
}

void BillboardNode::Render(Scene* pScene)
{
    g_BillboardShader->Bind();

    if (m_Material.bUseTexture)
    {
        const uint32_t texUnit = 0;
        m_Texture->BindUnit(texUnit);
        g_BillboardShader->SetUniform1i("u_Texture", texUnit);
    }

    g_BillboardShader->SetUniform3f("u_Position", m_Position);

    m_VertexArray->Bind();
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}

////////////////////////////////////////////////////
//  TerrainNode Implementation
////////////////////////////////////////////////////

TerrainNode::TerrainNode()
{
    int heightMapChannelCount;
    uint8_t* pHeightMapData = stbi_load("Assets/Heightmaps/HeightMap1.png", &m_HeightMapWidth, &m_HeightMapHeight, &heightMapChannelCount, 0);

    const float dim = 1.0f;
    std::vector<Vertex> vertices;

    for (uint32_t i = 0; i < m_HeightMapHeight; i++)
    {
        for (uint32_t j = 0; j < m_HeightMapWidth; j++)
        {
            const long long heightMapDataOffset = (i * m_HeightMapWidth + j) * heightMapChannelCount;
            const uint8_t colorR = pHeightMapData[heightMapDataOffset + 0];
            const uint8_t colorG = pHeightMapData[heightMapDataOffset + 1];
            const uint8_t colorB = pHeightMapData[heightMapDataOffset + 2];

            const uint32_t colorRGB = ((colorR & 0xff) << 16) | 
                                      ((colorG & 0xff) << 8 ) | 
                                      ((colorB & 0xff));

            const uint32_t magic = 16'777'216;
            
            float height = colorRGB / (float)magic;
            height *= 40.0f;

            const float x = ((-m_HeightMapWidth / 2.0f) + j) * dim;
            const float y = height - 25.0f;
            const float z = ((-m_HeightMapHeight / 2.0f) + i) * dim;

            m_HeightPointValues.push_back(y);

            Vertex vertex;
            vertex.Pos = glm::vec3(x, y, z); vertex.Uv = glm::vec2((float)j / ((float)m_HeightMapWidth - 1.0f), (float)i / ((float)m_HeightMapHeight - 1.0f));
            vertices.push_back(vertex);
        }
    }

    stbi_image_free(pHeightMapData);

    for (uint32_t i = 0; i < m_HeightMapHeight; i++)
    {
        for (uint32_t j = 0; j < m_HeightMapWidth; j++)
        {
            float hL = HeightAt((j - 1 + m_HeightMapHeight) % m_HeightMapHeight, i    );
            float hR = HeightAt((j + 1 + m_HeightMapHeight) % m_HeightMapHeight, i    );
            float hD = HeightAt(j    , (i - 1 + m_HeightMapHeight) % m_HeightMapHeight);
            float hU = HeightAt(j    , (i + 1 + m_HeightMapHeight) % m_HeightMapHeight);

            vertices[i * m_HeightMapWidth + j].Normal = glm::normalize(glm::vec3(hL - hR, dim * 10.0f, hD - hU));
        }
    }

    std::vector<uint32_t> indices;

    for (int i = 0; i < (m_HeightMapHeight - 1); i++)
    {
        for (int j = 0; j < (m_HeightMapWidth - 1); j++)
        {            
            indices.push_back((i + 0) * m_HeightMapWidth + j + 0);
            indices.push_back((i + 1) * m_HeightMapWidth + j + 0);
            indices.push_back((i + 0) * m_HeightMapWidth + j + 1);
            
            indices.push_back((i + 0) * m_HeightMapWidth + j + 1);
            indices.push_back((i + 1) * m_HeightMapWidth + j + 0);
            indices.push_back((i + 1) * m_HeightMapWidth + j + 1);
        }
    }

    m_VertexArray.reset(new VertexArray());
    m_VertexArray->SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_VertexArray->SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_VertexArray->SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);

    const ssize_t vertexBufferSize = sizeof(Vertex) * vertices.size();
    m_VertexBuffer.reset(new VertexBuffer(vertexBufferSize, GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, vertexBufferSize, vertices.data());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    vertices.clear();

    const ssize_t indexBufferSize = sizeof(uint32_t) * indices.size();
    m_IndexBuffer.reset(new IndexBuffer(indexBufferSize, GL_STATIC_DRAW));
    m_IndexBuffer->MapMemory(0, indexBufferSize, indices.data());
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    m_IndexCount = indices.size();
    indices.clear();
}

TerrainNode::~TerrainNode()
{
}

void TerrainNode::Render(Scene* pScene)
{
    g_TerrainShader->Bind();

    g_TerrainShader->SetUniformMatrix4f("u_World", glm::mat4(1.0f));

    g_TerrainShader->SetUniform4f("u_Material.Ambient", m_Material.Ambient);
    g_TerrainShader->SetUniform4f("u_Material.Diffuse", m_Material.Diffuse);
    g_TerrainShader->SetUniform4f("u_Material.Specular", m_Material.Specular);
    g_TerrainShader->SetUniform4f("u_Material.Emissive", m_Material.Emissive);
    g_TerrainShader->SetUniform1f("u_Material.SpecularPower", m_Material.SpecularPower);
    g_TerrainShader->SetUniform1b("u_Material.bUseTexture", m_Material.bUseTexture);

    m_Material.bUseTexture = true;
    if (m_Material.bUseTexture)
    {
        pScene->GetTexture("Assets/Heightmaps/BlendMap.png")->BindUnit(0);
        g_TerrainShader->SetUniform1i("u_BlendMapTexture", 0);

        pScene->GetTexture("Assets/Textures/Terrain/ElwynnDirtBase.png")->BindUnit(1);
        g_TerrainShader->SetUniform1i("u_DirtBaseTexture", 1);

        pScene->GetTexture("Assets/Textures/Terrain/ElwynnCobblestoneBase.png")->BindUnit(2);
        g_TerrainShader->SetUniform1i("u_StonebrickTexture", 2);

        pScene->GetTexture("Assets/Textures/Terrain/ElwynnGrassBase.png")->BindUnit(3);
        g_TerrainShader->SetUniform1i("u_GrassTexture", 3);

        pScene->GetTexture("Assets/Textures/Terrain/ElwynnCrop.png")->BindUnit(4);
        g_TerrainShader->SetUniform1i("u_CropTex", 4);
    }

    m_VertexArray->Bind();
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}

float TerrainNode::HeightAt(float x, float z)
{
    if (!(0 <= (int)x && (int)x <= m_HeightMapWidth)
    || !(0 <= (int)z && (int)z <= m_HeightMapHeight))
        return 0.0f;

    const float height = m_HeightPointValues[z * m_HeightMapWidth + x];
    return height;
}