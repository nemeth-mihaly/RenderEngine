#include "SceneNodes.h"

#include "Application.h"
#include "Scene.h"

//-----------------------------------------------------------------------------
// SceneNode Implementation
//-----------------------------------------------------------------------------

SceneNode::SceneNode()
{
    m_name = "SceneNode";

    m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

SceneNode::~SceneNode()
{
}

void SceneNode::Init(Scene& scene)
{
    SceneNodeVector::iterator it = m_children.begin();

    while (it != m_children.end())
    {
        (*it)->m_transform = glm::translate(glm::mat4(1.0f), (*it)->m_pos);
        it++;
    }
}

void SceneNode::Update(Scene& scene, const float deltaTime)
{
    if (m_pParent)
    {
        m_transform = glm::translate(glm::mat4(1.0f), m_pos)
        * glm::toMat4(m_rotation)
        * glm::scale(glm::mat4(1.0f), m_scale);
        
        m_transform = m_pParent->m_transform * m_transform;
    }
    else
    {
        m_transform = glm::translate(glm::mat4(1.0f), m_pos)
        * glm::toMat4(m_rotation)
        * glm::scale(glm::mat4(1.0f), m_scale);
    }

    SceneNodeVector::iterator it = m_children.begin();

    while (it != m_children.end())
    {
        (*it)->Update(scene, deltaTime);
        it++;
    }
}

void SceneNode::Render(Scene& scene)
{
}

void SceneNode::RenderChildren(Scene& scene)
{
    SceneNodeVector::iterator it = m_children.begin();

    while (it != m_children.end())
    {
        const float alpha = (*it)->GetMaterial().Diffuse.a;

        if (alpha == 1.0f)
        {
            (*it)->Render(scene);
        }
        else 
        if (alpha == 0.0f)
        {
            AlphaSceneNode* pAlphaSceneNode = new AlphaSceneNode();
            pAlphaSceneNode->node = (*it);
            
            scene.AddAlphaSceneNode(pAlphaSceneNode);
        }

        (*it)->RenderChildren(scene);
        it++;
    }
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> node)
{
    m_children.push_back(node);
    node->m_pParent = this;
}

////////////////////////////////////////////////////
//  CameraNode Implementation
////////////////////////////////////////////////////

CameraNode::CameraNode()
{
    m_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    m_ForwardDir = glm::vec3(0.0f, 0.0f, -1.0f);
    m_TargetPos = m_pos + m_ForwardDir;
    m_Projection = glm::perspective(glm::radians(45.0f), (1280 / static_cast<float>(720)), 0.001f, 1000.0f);
    WorldViewProjection();
}

CameraNode::~CameraNode()
{
}

glm::mat4 CameraNode::WorldViewProjection()
{
    //m_View = glm::lookAt(m_Pos, (m_Pos + m_ForwardDir), glm::vec3(0.0f, 1.0f, 0.0f));
    m_View = glm::lookAt(m_pos, m_TargetPos, glm::vec3(0.0f, 1.0f, 0.0f));
    return (m_Projection * m_View);
}

////////////////////////////////////////////////////
//  MeshNode Implementation
////////////////////////////////////////////////////

MeshNode::MeshNode(const StrongMeshPtr& mesh, const StrongShaderPtr& shader, const StrongTexturePtr& texture)
    : m_Mesh(mesh), m_Shader(shader), m_Texture(texture)
{
    m_meshResource = "";
}

MeshNode::~MeshNode()
{
}

void MeshNode::Init(Scene& scene)
{
}

void MeshNode::Render(Scene& scene)
{
    m_Shader->Bind();

    m_Shader->SetUniformMatrix4f("u_World", m_transform);

    m_Shader->SetUniform4f("u_Material.Ambient", m_material.Ambient);
    m_Shader->SetUniform4f("u_Material.Diffuse", m_material.Diffuse);
    m_Shader->SetUniform4f("u_Material.Specular", m_material.Specular);
    m_Shader->SetUniform4f("u_Material.Emissive", m_material.Emissive);
    m_Shader->SetUniform1f("u_Material.SpecularPower", m_material.SpecularPower);
    m_Shader->SetUniform1b("u_Material.bUseTexture", m_material.bUseTexture);

    if (m_material.bUseTexture)
    {
        const uint32_t textureUnit = 0;
        m_Texture->BindUnit(textureUnit);
        m_Shader->SetUniform1i("u_Texture", textureUnit);
    }

    //std::shared_ptr<Mesh> mesh = scene.GetMesh(m_meshResource);

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

    const signed long long vertexBufferSize = sizeof(Vertex) * vertices.size();
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

void CubeMapNode::Render(Scene& scene)
{
    glDepthFunc(GL_LEQUAL);

    g_SkyShader->Bind();
    g_SkyShader->SetUniformMatrix4f("u_WorldView", glm::mat4(glm::mat3(scene.GetCamera()->GetView())));
    g_SkyShader->SetUniformMatrix4f("u_WorldProjection", scene.GetCamera()->GetProjection());

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
    scale = 1.0f;

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

    const signed long long vertexBufferSize = sizeof(Vertex) * vertices.size();
    m_VertexBuffer.reset(new VertexBuffer(vertexBufferSize, GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, vertexBufferSize, vertices.data());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    vertices.clear();

    const signed long long indexBufferSize = sizeof(uint32_t) * indices.size();
    m_IndexBuffer.reset(new IndexBuffer(indexBufferSize, GL_STATIC_DRAW));
    m_IndexBuffer->MapMemory(0, indexBufferSize, indices.data());
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    m_IndexCount = indices.size();
    indices.clear();
}

BillboardNode::~BillboardNode()
{
}

void BillboardNode::Render(Scene& scene)
{
    g_BillboardShader->Bind();

    if (m_material.bUseTexture)
    {
        const uint32_t texUnit = 0;
        m_Texture->BindUnit(texUnit);
        g_BillboardShader->SetUniform1i("u_Texture", texUnit);
    }

    m_pos = m_pParent->GetPosition();
    g_BillboardShader->SetUniform3f("u_Position", m_pos);
    g_BillboardShader->SetUniform1f("scale", scale);

    m_VertexArray->Bind();
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}

////////////////////////////////////////////////////
//  TerrainNode Implementation
////////////////////////////////////////////////////

TerrainNode::TerrainNode()
{
    printf("Generating terrain..\n");

    stbi_set_flip_vertically_on_load(0);

    int heightMapChannelCount;
    uint8_t* pHeightMapData = stbi_load("Assets/Heightmaps/HeightMap1.png", &m_HeightMapWidth, &m_HeightMapHeight, &heightMapChannelCount, 0);

    const float dim = 1.0f;
    std::vector<Vertex> vertices;

    for (int i = 0; i < m_HeightMapHeight; i++)
    {
        for (int j = 0; j < m_HeightMapWidth; j++)
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
            height *= 85.0f;

            const float x = ((-m_HeightMapWidth / 2.0f) + j) * dim;
            const float y = height - 20.0f;
            const float z = ((-m_HeightMapHeight / 2.0f) + i) * dim;

            m_HeightPointValues.push_back(y);

            Vertex vertex;
            vertex.Pos = glm::vec3(x, y, z); vertex.Uv = glm::vec2((float)j / ((float)m_HeightMapWidth - 1.0f), (float)i / ((float)m_HeightMapHeight - 1.0f));
            vertices.push_back(vertex);
        }
    }

    stbi_image_free(pHeightMapData);

    for (int i = 0; i < m_HeightMapHeight; i++)
    {
        for (int j = 0; j < m_HeightMapWidth; j++)
        {
            float hL = HeightAt((j - 1 + m_HeightMapHeight) % m_HeightMapHeight, i    );
            float hR = HeightAt((j + 1 + m_HeightMapHeight) % m_HeightMapHeight, i    );
            float hD = HeightAt(j    , (i - 1 + m_HeightMapHeight) % m_HeightMapHeight);
            float hU = HeightAt(j    , (i + 1 + m_HeightMapHeight) % m_HeightMapHeight);

            vertices[i * m_HeightMapWidth + j].Normal = glm::normalize(glm::vec3(hL - hR, dim * 4.0f, hD - hU));
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

    const signed long long vertexBufferSize = sizeof(Vertex) * vertices.size();
    m_VertexBuffer.reset(new VertexBuffer(vertexBufferSize, GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, vertexBufferSize, vertices.data());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    vertices.clear();

    const signed long long indexBufferSize = sizeof(uint32_t) * indices.size();
    m_IndexBuffer.reset(new IndexBuffer(indexBufferSize, GL_STATIC_DRAW));
    m_IndexBuffer->MapMemory(0, indexBufferSize, indices.data());
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    m_IndexCount = indices.size();
    indices.clear();
}

TerrainNode::~TerrainNode()
{
    printf("Destroying terrain..\n");
}

void TerrainNode::Render(Scene& scene)
{
    g_TerrainShader->Bind();

    g_TerrainShader->SetUniformMatrix4f("u_World", glm::mat4(1.0f));

    g_TerrainShader->SetUniform4f("u_Material.Ambient", m_material.Ambient);
    g_TerrainShader->SetUniform4f("u_Material.Diffuse", m_material.Diffuse);
    g_TerrainShader->SetUniform4f("u_Material.Specular", m_material.Specular);
    g_TerrainShader->SetUniform4f("u_Material.Emissive", m_material.Emissive);
    g_TerrainShader->SetUniform1f("u_Material.SpecularPower", m_material.SpecularPower);
    g_TerrainShader->SetUniform1b("u_Material.bUseTexture", m_material.bUseTexture);

    m_material.bUseTexture = true;
    if (m_material.bUseTexture)
    {
        scene.GetTexture("Assets/Heightmaps/BlendMap.png")->BindUnit(0);
        g_TerrainShader->SetUniform1i("u_BlendMapTexture", 0);

        scene.GetTexture("Assets/Textures/Terrain/ElwynnDirtBase.png")->BindUnit(1);
        g_TerrainShader->SetUniform1i("u_DirtBaseTexture", 1);

        scene.GetTexture("Assets/Textures/Terrain/ElwynnCobblestoneBase.png")->BindUnit(2);
        g_TerrainShader->SetUniform1i("u_StonebrickTexture", 2);

        scene.GetTexture("Assets/Textures/Terrain/ElwynnGrassBase.png")->BindUnit(3);
        g_TerrainShader->SetUniform1i("u_GrassTexture", 3);

        scene.GetTexture("Assets/Textures/Terrain/ElwynnCrop.png")->BindUnit(4);
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

//-----------------------------------------------------------------------------
// LampNode Implementation
//-----------------------------------------------------------------------------

LampNode::LampNode()
{
    m_elapsedTimeInSeconds = 0.0f;
    m_glowScale = 1.0f;
}

LampNode::~LampNode()
{
}

void LampNode::Init(Scene& scene)
{
    m_lamp.reset(new MeshNode(scene.GetMesh("Assets/Models/Cube.obj"), g_shader_LitTextured, scene.GetTexture("Assets/Textures/UvGrid.png")));
    m_lamp->SetScale({0.2f, 0.2f, 0.2f});
    AddChild(m_lamp);

    Material alphaMaterial;
    alphaMaterial.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    alphaMaterial.Emissive = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    alphaMaterial.bUseTexture = true;

    m_glow.reset(new BillboardNode(scene.GetTexture("Assets/Textures/SphereGlow.png")));
    m_glow->SetMaterial(alphaMaterial);
    AddChild(m_glow);
}

void LampNode::Update(Scene& scene, const float deltaTime)
{
    SceneNode::Update(scene, deltaTime);

    m_elapsedTimeInSeconds += deltaTime;
    m_glowScale = m_glowScale + sinf(m_elapsedTimeInSeconds * 2.0f) * 0.002f;
    m_glow->scale = m_glowScale;
}

void LampNode::Render(Scene& scene)
{
}