#include "Scene.h"

#include "Application.h"

StrongShaderPtr     g_TerrainShader = nullptr;

////////////////////////////////////////////////////
//  Scene Implementation
////////////////////////////////////////////////////

Scene::Scene()
{
    g_TerrainShader.reset(new Shader());
    g_TerrainShader->LoadFromFile("Assets/Shaders/Terrain.vert", "Assets/Shaders/Terrain.frag");

    /** Main Camera */

    m_Camera.reset(new CameraNode());
    assert(m_Camera != NULL);
    m_Camera->m_name = "Camera";

    m_SceneNodes.push_back(m_Camera);

    /** Monkey */

    Material material;
    material.bUseTexture = true;

    std::shared_ptr<SceneNode> suzanneTheMonkey(new MeshNode(GetMesh("Assets/Models/Monkey.obj"), g_TexturedLitShader, GetTexture("Assets/Textures/UvGrid.png")));
    suzanneTheMonkey->SetPosition(glm::vec3(0.0f, 1.0f, -10.0f));
    suzanneTheMonkey->SetMaterial(material);
    suzanneTheMonkey->m_name = "Monkey";
    
    m_SceneNodes.push_back(suzanneTheMonkey);

    m_Camera->m_TargetNode = suzanneTheMonkey;

    /** Billboard */

    Material alphaMaterial;
    alphaMaterial.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    alphaMaterial.Emissive = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    alphaMaterial.bUseTexture = true;
    
    std::shared_ptr<BillboardNode> billboard(new BillboardNode(GetTexture("Assets/Textures/SphereGlow.png")));
    billboard->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    billboard->SetMaterial(alphaMaterial);
    billboard->m_name = "Fake Glow";

    m_SceneNodes.push_back(billboard);

    m_Terrain.reset(new TerrainNode());
    m_Terrain->m_name = "Terrain";
    m_SceneNodes.push_back(m_Terrain);

    /** Directional Light */

    LightProperties DirectionalLightProperties;
    DirectionalLightProperties.Type = LightType::Directional;
    DirectionalLightProperties.Direction = glm::vec3(1.0f, -1.0f, 1.3f);

    std::shared_ptr<LightNode> DirectionalLight(new LightNode(DirectionalLightProperties));
    DirectionalLight->m_name = "Directional Light";
    m_SceneNodes.push_back(DirectionalLight);
    m_LightNodes.push_back(DirectionalLight);

    /** Point Light */

    LightProperties PointLightProperties;
    PointLightProperties.Type = LightType::Point;
    PointLightProperties.Position = glm::vec3(0.0f, 0.2f, -2.0f);
    PointLightProperties.ConstantAttenuation = 1.0f;
    PointLightProperties.LinearAttenuation = 0.09f;
    PointLightProperties.QuadraticAttenuation = 0.032f;

    std::shared_ptr<LightNode> PointLight(new LightNode(PointLightProperties));
    PointLight->SetPosition(glm::vec3(-0.4f, 0.5f, -1.0f));
    PointLight->m_name = "Point Light";

    m_SceneNodes.push_back(PointLight);
    m_LightNodes.push_back(PointLight);

    /** Spot Light */

    LightProperties SpotLightProperties;
    SpotLightProperties.Type = LightType::Spot;
    SpotLightProperties.Position = glm::vec3(3.4f, 1.0f, 3.0f);
    SpotLightProperties.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    SpotLightProperties.ConstantAttenuation = 1.0f;
    SpotLightProperties.LinearAttenuation = 0.09f;
    SpotLightProperties.QuadraticAttenuation = 0.032f;
    SpotLightProperties.Falloff = cosf(glm::radians(30.0f));
    SpotLightProperties.Phi = cosf(glm::radians(36.0f));

    std::shared_ptr<LightNode> SpotLight(new LightNode(SpotLightProperties));
    SpotLight->SetPosition(glm::vec3(0.4f, 1.0f, -2.0f));
    SpotLight->m_name = "Spot Light";

    m_SceneNodes.push_back(SpotLight);
    m_LightNodes.push_back(SpotLight);

    /** Sky */

    m_CubeMap.reset(new CubeMapNode());

    m_UniformBufferMatrices.reset(new UniformBuffer(0, 2 * sizeof(glm::mat4), GL_DYNAMIC_DRAW));
    m_UniformBufferLighting.reset(new UniformBuffer(1, 32 * sizeof(LightProperties), GL_DYNAMIC_DRAW));

    g_TexturedLitShader->SetUniformBlockBinding(0, "Matrices");
    g_SkyShader->SetUniformBlockBinding(0, "Matrices");
    g_BillboardShader->SetUniformBlockBinding(0, "Matrices");
    g_TerrainShader->SetUniformBlockBinding(0, "Matrices");
    g_pShader_UnlitColored->SetUniformBlockBinding(0, "Matrices");

    g_TexturedLitShader->SetUniformBlockBinding(1, "Lighting");
    g_TerrainShader->SetUniformBlockBinding(1, "Lighting");

    /** Particles */

    /*
    std::vector<Vertex> particleVertices =
    {
        {{ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
        {{  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},

        {{  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
        {{  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }}, 
    };

    m_ParticleVertexBuffer.reset(new VertexBuffer(sizeof(Vertex) * particleVertices.size(), GL_STATIC_DRAW));
    m_ParticleVertexBuffer->MapMemory(0, sizeof(Vertex) * particleVertices.size(), particleVertices.data());
    m_ParticleVertexCount = particleVertices.size();
    particleVertices.clear();

    m_ParticleExtraDataVertexBuffer.reset(new VertexBuffer(sizeof(ParticleExtraVertexData) * MAX_PARTICLES, GL_DYNAMIC_DRAW));

    m_ParticleVertexArray.reset(new VertexArray());

    m_ParticleVertexArray->SetVertexBuffer(0, m_ParticleVertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    m_ParticleVertexArray->SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_ParticleVertexArray->SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_ParticleVertexArray->SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);

    m_ParticleVertexArray->SetVertexBuffer(1, m_ParticleExtraDataVertexBuffer, sizeof(ParticleExtraVertexData), VertexArrayInputRate_Instance);
    m_ParticleVertexArray->SetVertexAttribute(1, 3, 3, GL_FLOAT, 0);

    m_ParticleStagingBuffer.reserve(MAX_PARTICLES);

    int i = 0;
//    for (uint32_t i = 0; i < MAX_PARTICLES; i++)
    {
        m_Particles[i].Pos = glm::vec3(0.0f, 0.0f, -5.0f);
        m_Particles[i].Velocity = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Particles[i].Life = 10.0f;
    }
    */
}

Scene::~Scene()
{
}

void Scene::Update(const float deltaTime)
{
    m_Camera->WorldViewProjection();

    for (const std::shared_ptr<SceneNode>& node : m_SceneNodes)
    {
        node->Update(this, deltaTime);
    }

    /*
    for (uint32_t i = 0; i < MAX_PARTICLES; i++)
    {
        Particle& particle = m_Particles[i];

        if (particle.Life > 0.0f)
        {
            particle.Pos += particle.Velocity * deltaTime;
            particle.Life -= deltaTime;

            ParticleExtraVertexData extraData;
            extraData.Pos = particle.Pos;
            //extraData.Size = 1.0f;

            m_ParticleStagingBuffer.push_back(extraData);
        }
    }
    */

    glm::vec3 cameraTargetPos = m_Camera->m_TargetNode->GetPosition();
    cameraTargetPos.y = m_Terrain->HeightAt(cameraTargetPos.x, cameraTargetPos.z) + 0.5f;
    m_Camera->m_TargetNode->SetPosition(cameraTargetPos);
}

void Scene::Render()
{
    m_UniformBufferMatrices->MapMemory(0, sizeof(glm::mat4), glm::value_ptr(m_Camera->GetProjection()));
    m_UniformBufferMatrices->MapMemory(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_Camera->GetView()));

    /** Actor Pass */

    glEnable(GL_DEPTH_TEST);
    //glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_TexturedLitShader->Bind();

    for (uint32_t i = 0; i < m_LightNodes.size(); i++)
    {
        const std::string strIndex = std::to_string(i);

        const Material& material = m_LightNodes[i]->GetMaterial();
        const LightProperties& lightProperties = m_LightNodes[i]->GetProperties();

        g_TexturedLitShader->SetUniform1i(("u_Lights[" + strIndex + "].Type"), static_cast<int>(lightProperties.Type));
        g_TexturedLitShader->SetUniform3f(("u_Lights[" + strIndex + "].Position"), lightProperties.Position);
        g_TexturedLitShader->SetUniform3f(("u_Lights[" + strIndex + "].Direction"), lightProperties.Direction);
        g_TexturedLitShader->SetUniform4f(("u_Lights[" + strIndex + "].Ambient"), material.Ambient);
        g_TexturedLitShader->SetUniform4f(("u_Lights[" + strIndex + "].Diffuse"), material.Diffuse);
        g_TexturedLitShader->SetUniform4f(("u_Lights[" + strIndex + "].Specular"), material.Specular);
        g_TexturedLitShader->SetUniform1f(("u_Lights[" + strIndex + "].Falloff"), lightProperties.Falloff);
        g_TexturedLitShader->SetUniform1f(("u_Lights[" + strIndex + "].ConstantAttenuation"), lightProperties.ConstantAttenuation);
        g_TexturedLitShader->SetUniform1f(("u_Lights[" + strIndex + "].LinearAttenuation"), lightProperties.LinearAttenuation);
        g_TexturedLitShader->SetUniform1f(("u_Lights[" + strIndex + "].QuadraticAttenuation"), lightProperties.QuadraticAttenuation);
    }

    g_TerrainShader->Bind();

    for (uint32_t i = 0; i < m_LightNodes.size(); i++)
    {
        const std::string strIndex = std::to_string(i);

        const Material& material = m_LightNodes[i]->GetMaterial();
        const LightProperties& lightProperties = m_LightNodes[i]->GetProperties();

        g_TerrainShader->SetUniform1i(("u_Lights[" + strIndex + "].Type"), static_cast<int>(lightProperties.Type));
        g_TerrainShader->SetUniform3f(("u_Lights[" + strIndex + "].Position"), lightProperties.Position);
        g_TerrainShader->SetUniform3f(("u_Lights[" + strIndex + "].Direction"), lightProperties.Direction);
        g_TerrainShader->SetUniform4f(("u_Lights[" + strIndex + "].Ambient"), material.Ambient);
        g_TerrainShader->SetUniform4f(("u_Lights[" + strIndex + "].Diffuse"), material.Diffuse);
        g_TerrainShader->SetUniform4f(("u_Lights[" + strIndex + "].Specular"), material.Specular);
        g_TerrainShader->SetUniform1f(("u_Lights[" + strIndex + "].Falloff"), lightProperties.Falloff);
        g_TerrainShader->SetUniform1f(("u_Lights[" + strIndex + "].ConstantAttenuation"), lightProperties.ConstantAttenuation);
        g_TerrainShader->SetUniform1f(("u_Lights[" + strIndex + "].LinearAttenuation"), lightProperties.LinearAttenuation);
        g_TerrainShader->SetUniform1f(("u_Lights[" + strIndex + "].QuadraticAttenuation"), lightProperties.QuadraticAttenuation);
    }

    for (const std::shared_ptr<SceneNode>& sceneNode : m_SceneNodes)
    {
        const float alpha = sceneNode->GetMaterial().Diffuse.a;

        if (alpha == 1.0f)
        {
            sceneNode->Render(this);
        }
        else if (alpha == 0.0f)
        {
            AlphaNode* pAlphaNode = new AlphaNode();
            pAlphaNode->Node = sceneNode;

            m_AlphaNodes.push_back(pAlphaNode);
        }
    }

    //  Draw Sky Node

    m_CubeMap->Render(this);

    //  Draw Alpha Nodes

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (uint32_t i = 0; i < m_AlphaNodes.size(); i++)
    {
        for (uint32_t j = (i + 1); j < m_AlphaNodes.size(); j++)
        {
            auto Vec3Length = [](const glm::vec3& p) -> float
            {
                return (p.x * p.x + p.y * p.y + p.z * p.z);
            };

            if (Vec3Length(m_Camera->GetPosition() - m_AlphaNodes[i]->Node->GetPosition()) 
            > Vec3Length(m_Camera->GetPosition() - m_AlphaNodes[j]->Node->GetPosition()))
            {
                AlphaNode* pTempAlphaNode = new AlphaNode();
                pTempAlphaNode->Node = m_AlphaNodes[i]->Node;

                m_AlphaNodes[i] = m_AlphaNodes[j];
                m_AlphaNodes[j] = pTempAlphaNode;
            }
        }
    }

    for (int i = (m_AlphaNodes.size() - 1); i >= 0; i--)
    {
        m_AlphaNodes[i]->Node->Render(this);
        delete m_AlphaNodes[i];
    }

    m_AlphaNodes.clear();

    glDisable(GL_BLEND);

    /** Particles */

    /*
    g_BillboardShader->Bind();

    uint32_t textureUnit = 0;
    g_SphereGlowTexture->BindUnit(textureUnit);
    g_BillboardShader->SetUniform1i("u_Texture", textureUnit);

    m_ParticleExtraDataVertexBuffer->MapMemory(0, sizeof(ParticleExtraVertexData) * m_ParticleStagingBuffer.size(), m_ParticleStagingBuffer.data());
    m_ParticleVertexArray->Bind();

    glDrawArraysInstanced(GL_TRIANGLES, 0, m_ParticleVertexCount, m_ParticleStagingBuffer.size());
    m_ParticleStagingBuffer.clear();
    */
}

StrongMeshPtr Scene::GetMesh(const std::string& name)
{
    StrongMeshPtr mesh(nullptr);

    std::unordered_map<std::string, StrongMeshPtr>::iterator it = m_Meshes.find(name);
    if (it != m_Meshes.end())
        mesh = (*it).second;
    else
    {
        mesh.reset(new Mesh());
        mesh->LoadResource(name);

        m_Meshes[name] = mesh;
    }

    return mesh;
}

StrongTexturePtr Scene::GetTexture(const std::string& name)
{
    StrongTexturePtr texture(nullptr);

    std::unordered_map<std::string, StrongTexturePtr>::iterator it = m_Textures.find(name);
    if (it != m_Textures.end())
        texture = (*it).second;
    else
    {
        texture.reset(new Texture(GL_TEXTURE_2D, GL_REPEAT, GL_LINEAR));
        texture->LoadResource(name);

        m_Textures[name] = texture;
    }

    return texture;
}

void Scene::ReloadTerrain()
{
    for (std::list<std::shared_ptr<SceneNode>>::iterator i = m_SceneNodes.begin(); i != m_SceneNodes.end(); i++)
    {
        if ((*i) == m_Terrain)
            i = m_SceneNodes.erase(i);
    }

    m_Terrain.reset(new TerrainNode());
    m_SceneNodes.push_back(m_Terrain);
}