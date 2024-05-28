#include "Scene.h"

#include "Application.h"

////////////////////////////////////////////////////
//  Scene Implementation
////////////////////////////////////////////////////

Scene::Scene()
{
    /** Main Camera */

    m_Camera.reset(new CameraNode());
    assert(m_Camera != NULL);

    m_SceneNodes.push_back(m_Camera);

    /** Monkey */

    Material Material;
    Material.bUseTexture = true;

    std::shared_ptr<MeshSceneNode> suzanneTheMonkey(new MeshSceneNode("Assets/Models/Monkey.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    suzanneTheMonkey->SetPosition(glm::vec3(0.0f, 1.0f, -5.0f));
    suzanneTheMonkey->SetMaterial(Material);
    m_SceneNodes.push_back(suzanneTheMonkey);

    //auto floorNode = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Cube.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //floorNode->Create();
    //floorNode->SetPosition(glm::vec3(0.0f, -0.55f, -0.0f));
    //floorNode->SetScale(glm::vec3(10.0f, 0.1f, 10.0f));
    //floorNode->GetMaterial().Diffuse = glm::vec4(0.0f, 0.7f, 0.0f, 1.0f);
    //floorNode->GetMaterial().Specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    //floorNode->GetMaterial().bUseTexture = false;

    //auto node1 = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Cube.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //node1->Create();
    //node1->SetPosition(glm::vec3(-1.5f, 0.0f, -2.5f));
    //node1->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //node1->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //node1->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //node1->GetMaterial().bUseTexture = true;

    //auto node2 = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Cube.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //node2->Create();
    //node2->SetPosition(glm::vec3(2.0f, 0.0f, -2.5f));
    //node2->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //node2->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //node2->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //node2->GetMaterial().bUseTexture = true;

    //auto node3 = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Cube.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //node3->Create();
    //node3->SetPosition(glm::vec3(1.0f, 0.0f, -3.5f));
    //node3->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //node3->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //node3->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //node3->GetMaterial().bUseTexture = true;

    //auto monkeyNode = m_SceneNodes.emplace_back(new MeshSceneNode("Assets/Models/Monkey.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets/Textures/UvGrid.png"));
    //monkeyNode->Create();
    //monkeyNode->SetPosition(glm::vec3(0.0f, 1.0f, -5.0f));
    //monkeyNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //monkeyNode->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //monkeyNode->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //monkeyNode->GetMaterial().bUseTexture = true;

    //std::vector<glm::vec3> grassPositions =
    //{
    //    glm::vec3(-1.5f, 0.0f, -0.48f),
    //    glm::vec3( 1.5f, 0.0f, 0.51f),
    //    glm::vec3( 0.0f, 0.0f, 0.7f),
    //    glm::vec3(-0.3f, 0.0f, -2.3f),
    //    glm::vec3(0.5f, 0.0f, -0.6f)
    //};

    //for (uint32_t i = 0; i < grassPositions.size(); ++i)
    //{
    //    auto grassNode = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Grass.obj", "Assets/Shaders/TexturedLit.progpipeline", "Assets\\Textures\\Grass.png"));
    //    grassNode->Create();
    //    grassNode->SetPosition(grassPositions[i]);
    //    grassNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //    grassNode->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //    grassNode->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //    grassNode->GetMaterial().bUseTexture = true; 
    //}

    //std::vector<glm::vec3> glowPositions =
    //{
    //    glm::vec3(-1.5f, 0.3f, 0.48f),
    //    glm::vec3( 1.5f, 0.3f, 1.51f),
    //    glm::vec3( 0.0f, 0.3f, 1.7f),
    //    glm::vec3(-0.3f, 0.3f, 2.3f),
    //    glm::vec3(0.5f, 0.3f, 0.6f)
    //};

    //for (uint32_t i = 0; i < glowPositions.size(); ++i)
    //{
    //    std::string textureName = "Assets\\Textures\\SphereGlow.png";
//
    //    if (i % 2 == 0)
    //    {
    //        textureName = "Assets\\Textures\\AlphaWindow.png";
    //    }
//
    //    auto grassNode = m_SceneNodes.emplace_back(new MeshSceneNode("Assets\\Models\\Rectangle.obj", "Assets/Shaders/TexturedLit.progpipeline", textureName));
    //    grassNode->Create();
    //    grassNode->SetPosition(glowPositions[i]);
    //    grassNode->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //    grassNode->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
    //    grassNode->GetMaterial().Diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    //    grassNode->GetMaterial().Specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //    grassNode->GetMaterial().bUseTexture = true; 
    //}

    /** Directional Light */

    LightProperties DirectionalLightProperties;
    DirectionalLightProperties.Type = LightType::Directional;
    DirectionalLightProperties.Direction = glm::vec3(1.2f, -1.0f, 1.3f);

    std::shared_ptr<LightNode> DirectionalLight(new LightNode(DirectionalLightProperties));

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

    m_SceneNodes.push_back(SpotLight);
    m_LightNodes.push_back(SpotLight);

    /** Sky */

    m_SkyNode.reset(new SkyNode());
}

Scene::~Scene()
{
}

void Scene::Update(const float deltaTime)
{
    for (const std::shared_ptr<SceneNode>& node : m_SceneNodes)
    {
        node->Update(this, deltaTime);
    }
}

void Scene::Render()
{
    /** Actor Render Pass */

    glEnable(GL_DEPTH_TEST);
    //glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    glClearColor(0.05f, 0.05f, 0.05f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    g_TexturedLitShader->Bind();
    g_TexturedLitShader->SetUniformMatrix4f("u_WorldViewProjection", m_Camera->WorldViewProjection());
    g_TexturedLitShader->SetUniform3f("u_ViewPos", m_Camera->GetPosition());

    for (uint32_t i = 0; i < m_LightNodes.size(); ++i)
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

    /** Sky Render Pass */

    m_SkyNode->Render(this);

    /** Alpha Render Pass */

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

    /** Postproc Effects */
}