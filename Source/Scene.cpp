#include "Scene.h"

#include <iostream>

#include "Application.h"

StrongShaderPtr     g_TerrainShader = nullptr;

////////////////////////////////////////////////////
//  Scene Implementation
////////////////////////////////////////////////////

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
    g_TerrainShader.reset(new Shader());
    g_TerrainShader->LoadFromFile("Assets/Shaders/Terrain.vert", "Assets/Shaders/Terrain.frag");

    m_root.reset(new SceneNode());
    m_root->m_name = "Root";

    m_camera.reset(new CameraNode());
    m_camera->m_name = "Camera";
    //m_root->AddChild(m_camera);
    AddSceneNode(m_camera);

    std::shared_ptr<SceneNode> suzanneTheMonkey(new MeshNode(GetMesh("Assets/Models/Monkey.obj"), g_shader_LitTextured, GetTexture("Assets/Textures/UvGrid.png")));
    suzanneTheMonkey->SetPosition(glm::vec3(0.0f, 1.0f, -10.0f));
    suzanneTheMonkey->m_name = "Monkey";
    suzanneTheMonkey->SetRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    // suzanneTheMonkey->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
    //m_root->AddChild(suzanneTheMonkey);
    AddSceneNode(suzanneTheMonkey);

    m_camera->m_TargetNode = suzanneTheMonkey;

    std::shared_ptr<SceneNode> cube(new MeshNode(GetMesh("Assets/Models/Cube.obj"), g_shader_LitTextured, GetTexture("Assets/Textures/UvGrid.png")));
    cube->SetPosition({-5.0f, 0.0f, 0.0f});
    cube->m_name = "Child of Monkey";
    suzanneTheMonkey->AddChild(cube);

    std::shared_ptr<SceneNode> lamp(new LampNode());
    lamp->SetPosition({-5.0f, 5.0f, 0.0f});
    lamp->Init((*this));
    AddSceneNode(lamp);

    /** Billboard */

    Material alphaMaterial;
    alphaMaterial.Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    alphaMaterial.Emissive = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    alphaMaterial.bUseTexture = true;
    
    std::shared_ptr<BillboardNode> billboard(new BillboardNode(GetTexture("Assets/Textures/SphereGlow.png")));
    billboard->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
    billboard->SetMaterial(alphaMaterial);
    billboard->m_name = "Fake Glow";
    //m_root->AddChild(billboard);
    AddSceneNode(billboard);

    m_Terrain.reset(new TerrainNode());
    m_Terrain->m_name = "Terrain";
    //m_root->AddChild(m_Terrain);
    AddSceneNode(m_Terrain);

    LightProperties DirectionalLightProperties;
    DirectionalLightProperties.Type = LightType::Directional;
    DirectionalLightProperties.Direction = glm::vec3(1.0f, -1.0f, 1.3f);

    std::shared_ptr<LightNode> DirectionalLight(new LightNode(DirectionalLightProperties));
    DirectionalLight->m_name = "Directional Light";
    m_root->AddChild(DirectionalLight);
    m_LightNodes.push_back(DirectionalLight);

    LightProperties PointLightProperties;
    PointLightProperties.Type = LightType::Point;
    PointLightProperties.Position = glm::vec3(0.0f, 0.2f, -2.0f);
    PointLightProperties.ConstantAttenuation = 1.0f;
    PointLightProperties.LinearAttenuation = 0.09f;
    PointLightProperties.QuadraticAttenuation = 0.032f;

    std::shared_ptr<LightNode> PointLight(new LightNode(PointLightProperties));
    PointLight->SetPosition(glm::vec3(-0.4f, 0.5f, -1.0f));
    PointLight->m_name = "Point Light";
    //m_root->AddChild(PointLight);
    AddSceneNode(PointLight);
    m_LightNodes.push_back(PointLight);

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
    //m_root->AddChild(SpotLight);
    AddSceneNode(SpotLight);
    m_LightNodes.push_back(SpotLight);

    m_CubeMap.reset(new CubeMapNode());

    m_UniformBufferMatrices.reset(new UniformBuffer(0, 2 * sizeof(glm::mat4), GL_DYNAMIC_DRAW));
    m_UniformBufferLighting.reset(new UniformBuffer(1, 32 * sizeof(LightProperties), GL_DYNAMIC_DRAW));

    g_shader_LitTextured->SetUniformBlockBinding(0, "Matrices");
    g_SkyShader->SetUniformBlockBinding(0, "Matrices");
    g_BillboardShader->SetUniformBlockBinding(0, "Matrices");
    g_TerrainShader->SetUniformBlockBinding(0, "Matrices");
    g_shader_UnlitColored->SetUniformBlockBinding(0, "Matrices");

    g_shader_LitTextured->SetUniformBlockBinding(1, "Lighting");
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

void Scene::Update(const float deltaTime)
{
    m_camera->WorldViewProjection();

    // for (const std::shared_ptr<SceneNode>& node : m_SceneNodes)
    // {
    //     node->Update((*this), deltaTime);
    // }

    m_root->Update((*this), deltaTime);

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

    glm::vec3 cameraTargetPos = m_camera->m_TargetNode->GetPosition();
    cameraTargetPos.y = m_Terrain->HeightAt(cameraTargetPos.x, cameraTargetPos.z) + 0.5f;
    m_camera->m_TargetNode->SetPosition(cameraTargetPos);
}

void Scene::Render()
{
    m_UniformBufferMatrices->MapMemory(0, sizeof(glm::mat4), glm::value_ptr(m_camera->GetProjection()));
    m_UniformBufferMatrices->MapMemory(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera->GetView()));

    /** Actor Pass */

    glEnable(GL_DEPTH_TEST);
    //glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_shader_LitTextured->Bind();

    for (uint32_t i = 0; i < m_LightNodes.size(); i++)
    {
        const std::string strIndex = std::to_string(i);

        const Material& material = m_LightNodes[i]->GetMaterial();
        const LightProperties& lightProperties = m_LightNodes[i]->GetProperties();

        g_shader_LitTextured->SetUniform1i(("u_Lights[" + strIndex + "].Type"), static_cast<int>(lightProperties.Type));
        g_shader_LitTextured->SetUniform3f(("u_Lights[" + strIndex + "].Position"), lightProperties.Position);
        g_shader_LitTextured->SetUniform3f(("u_Lights[" + strIndex + "].Direction"), lightProperties.Direction);
        g_shader_LitTextured->SetUniform4f(("u_Lights[" + strIndex + "].Ambient"), material.Ambient);
        g_shader_LitTextured->SetUniform4f(("u_Lights[" + strIndex + "].Diffuse"), material.Diffuse);
        g_shader_LitTextured->SetUniform4f(("u_Lights[" + strIndex + "].Specular"), material.Specular);
        g_shader_LitTextured->SetUniform1f(("u_Lights[" + strIndex + "].Falloff"), lightProperties.Falloff);
        g_shader_LitTextured->SetUniform1f(("u_Lights[" + strIndex + "].ConstantAttenuation"), lightProperties.ConstantAttenuation);
        g_shader_LitTextured->SetUniform1f(("u_Lights[" + strIndex + "].LinearAttenuation"), lightProperties.LinearAttenuation);
        g_shader_LitTextured->SetUniform1f(("u_Lights[" + strIndex + "].QuadraticAttenuation"), lightProperties.QuadraticAttenuation);
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

    /*
    for (const std::shared_ptr<SceneNode>& sceneNode : m_SceneNodes)
    {
        const float alpha = sceneNode->GetMaterial().Diffuse.a;

        if (alpha == 1.0f)
        {
            sceneNode->Render((*this));
        }
        else if (alpha == 0.0f)
        {
            AlphaNode* pAlphaNode = new AlphaNode();
            pAlphaNode->Node = sceneNode;

            m_alphaNodes.push_back(pAlphaNode);
        }
    }
    */

    m_root->Render((*this));
    m_root->RenderChildren((*this));

    //  Draw Sky Node

    m_CubeMap->Render((*this));

    //  Draw Alpha Nodes

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (uint32_t i = 0; i < m_alphaNodes.size(); i++)
    {
        for (uint32_t j = (i + 1); j < m_alphaNodes.size(); j++)
        {
            auto Vec3Length = [](const glm::vec3& p) -> float
            {
                return (p.x * p.x + p.y * p.y + p.z * p.z);
            };

            if (Vec3Length(m_camera->GetPosition() - m_alphaNodes[i]->node->GetPosition()) 
            > Vec3Length(m_camera->GetPosition() - m_alphaNodes[j]->node->GetPosition()))
            {
                AlphaSceneNode* pTempAlphaNode = new AlphaSceneNode();
                pTempAlphaNode->node = m_alphaNodes[i]->node;

                m_alphaNodes[i] = m_alphaNodes[j];
                m_alphaNodes[j] = pTempAlphaNode;
            }
        }
    }

    for (int i = (m_alphaNodes.size() - 1); i >= 0; i--)
    {
        m_alphaNodes[i]->node->Render((*this));
        delete m_alphaNodes[i];
    }

    m_alphaNodes.clear();

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

void Scene::AddSceneNode(std::shared_ptr<SceneNode> node)
{
    m_root->AddChild(node);
}

void Scene::AddAlphaSceneNode(AlphaSceneNode* pAlphaSceneNode)
{
    m_alphaNodes.push_back(pAlphaSceneNode); 
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