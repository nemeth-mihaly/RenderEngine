#include "SceneNodes.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

#include "Application.h"

//-----------------------------------------------------------------------------
// SceneNode Implementation
//-----------------------------------------------------------------------------

SceneNode::SceneNode()
{
    m_pParent = nullptr;

    m_name = "SceneNode";

    m_transform = glm::mat4(1);

    m_position = glm::vec3(0, 0, 0);
    m_rotation = glm::quat(1, 0, 0, 0);
    m_size = glm::vec3(1, 1, 1);

    m_boundingSphereRadius = 0.0f;
}

SceneNode::~SceneNode()
{
    // assert(m_children.empty());
}

void SceneNode::Init(const Json& data)
{
    if (data.contains("Name"))
    {
        m_name = data["Name"].get<std::string>();
    }

    if (data.contains("Position"))
    {
        float x = 0;
        float y = 0;
        float z = 0;

        if (data["Position"].contains("x") 
        && data["Position"].contains("y") 
        && data["Position"].contains("z")) 
        {
            x = data["Position"]["x"].get<float>();
            y = data["Position"]["y"].get<float>();
            z = data["Position"]["z"].get<float>();
        }  

        m_position = glm::vec3(x, y, z);
    }

    if (data.contains("PitchYawRoll"))
    {
        float pitch = 0;
        float yaw = 0;
        float roll = 0;

        if (data["PitchYawRoll"].contains("x") 
        && data["PitchYawRoll"].contains("y") 
        && data["PitchYawRoll"].contains("z")) 
        {
            pitch = data["PitchYawRoll"]["x"].get<float>();
            yaw = data["PitchYawRoll"]["y"].get<float>();
            roll = data["PitchYawRoll"]["z"].get<float>();
        }  

        pitch = glm::clamp(pitch, -179.9f, 179.9f);
        yaw = glm::clamp(yaw, -89.9f, 89.9f);
        roll = glm::clamp(roll, -179.9f, 179.9f);

        glm::vec3 rotation(pitch, yaw, roll);
        m_rotation = glm::quat(glm::radians(rotation));
    }

    if (data.contains("Size"))
    {
        float x = 1;
        float y = 1;
        float z = 1;

        if (data["Size"].contains("x") 
        && data["Size"].contains("y") 
        && data["Size"].contains("z")) 
        {
            x = data["Size"]["x"].get<float>();
            y = data["Size"]["y"].get<float>();
            z = data["Size"]["z"].get<float>();
        }  

        m_size = glm::vec3(x, y, z);
    }

    if (data.contains("Ambient"))
    {
        glm::vec4 vec(1, 1, 1, 1);

        if (data["Ambient"].contains("x") 
        && data["Ambient"].contains("y") 
        && data["Ambient"].contains("z")
        && data["Ambient"].contains("w")) 
        {
            vec.x = data["Ambient"]["x"].get<float>();
            vec.y = data["Ambient"]["y"].get<float>();
            vec.z = data["Ambient"]["z"].get<float>();
            vec.w = data["Ambient"]["w"].get<float>();
        }  

        m_material.ambient = vec;
    }

    if (data.contains("Diffuse"))
    {
        glm::vec4 vec(1, 1, 1, 1);

        if (data["Diffuse"].contains("x") 
        && data["Diffuse"].contains("y") 
        && data["Diffuse"].contains("z")
        && data["Diffuse"].contains("w")) 
        {
            vec.x = data["Diffuse"]["x"].get<float>();
            vec.y = data["Diffuse"]["y"].get<float>();
            vec.z = data["Diffuse"]["z"].get<float>();
            vec.w = data["Diffuse"]["w"].get<float>();
        }  

        m_material.diffuse = vec;
    }

    if (data.contains("Specular"))
    {
        glm::vec4 vec(1, 1, 1, 1);

        if (data["Specular"].contains("x") 
        && data["Specular"].contains("y") 
        && data["Specular"].contains("z")
        && data["Specular"].contains("w")) 
        {
            vec.x = data["Specular"]["x"].get<float>();
            vec.y = data["Specular"]["y"].get<float>();
            vec.z = data["Specular"]["z"].get<float>();
            vec.w = data["Specular"]["w"].get<float>();
        }  

        m_material.specular = vec;
    }

    if (data.contains("Emissive"))
    {
        glm::vec4 vec(1, 1, 1, 1);

        if (data["Emissive"].contains("x") 
        && data["Emissive"].contains("y") 
        && data["Emissive"].contains("z")
        && data["Emissive"].contains("w")) 
        {
            vec.x = data["Emissive"]["x"].get<float>();
            vec.y = data["Emissive"]["y"].get<float>();
            vec.z = data["Emissive"]["z"].get<float>();
            vec.w = data["Emissive"]["w"].get<float>();
        }  

        m_material.emissive = vec;
    }

    if (data.contains("SpecularPower"))
    {
        m_material.specularPower = data["SpecularPower"];
    }
}

void SceneNode::PostInit()
{
    for (std::vector<std::shared_ptr<SceneNode>>::iterator it = m_children.begin(); it != m_children.end(); it++)
    {
        (*it)->PostInit();
    }
}

void SceneNode::Update(const float deltaTime)
{
    if (m_pParent)
    {
        m_transform = m_pParent->m_transform * glm::translate(glm::mat4(1), m_position) * glm::toMat4(m_rotation) * glm::scale(glm::mat4(1), m_size);
    }
    else
    {
        m_transform = glm::translate(glm::mat4(1), m_position) * glm::toMat4(m_rotation) * glm::scale(glm::mat4(1), m_size);
    }

    for (std::vector<std::shared_ptr<SceneNode>>::iterator it = m_children.begin(); it != m_children.end(); it++)
    {
        (*it)->Update(deltaTime);
    }
}

void SceneNode::Render()
{

}

void SceneNode::RenderChildren()
{
    for (std::vector<std::shared_ptr<SceneNode>>::iterator it = m_children.begin(); it != m_children.end(); it++)
    {
        float alphaValue = (*it)->m_material.diffuse.a;

        if (alphaValue == 1.0f)
        {
            (*it)->Render();
        }
        else if (alphaValue == 0.0f)
        {
            g_pApp->GetRenderer()->AddTransparentSceneNode((*it));
        }

        (*it)->RenderChildren();
    }
}

void SceneNode::AddChild(std::shared_ptr<SceneNode> node)
{
    if (strcmp(LightNode::g_type, node->GetType()) == 0)
    {
        g_pApp->GetRenderer()->AddLight(node);
    }

    node->m_pParent = this;
    m_children.push_back(node);
}

void SceneNode::RemoveChild(std::shared_ptr<SceneNode> node)
{
    if (strcmp(LightNode::g_type, node->GetType()) == 0)
    {
        g_pApp->GetRenderer()->RemoveLight(node);
    }

    for (std::vector<std::shared_ptr<SceneNode>>::iterator it = m_children.begin(); it != m_children.end(); it++)
    {
        if ((*it) != node)
        {
            continue;
        }

        m_children.erase(it);

        break;
    }
}

Json SceneNode::ToJSON()
{
    Json data;

    // if (!m_name.empty())
    {
        data["Name"] = m_name;
    }

    data["type"] = GetType();

    data["Position"]["x"] = m_position.x;
    data["Position"]["y"] = m_position.y;
    data["Position"]["z"] = m_position.z;

    glm::vec3 rotation = glm::degrees(glm::eulerAngles(m_rotation));
    data["PitchYawRoll"]["x"] = rotation.x;
    data["PitchYawRoll"]["y"] = rotation.y;
    data["PitchYawRoll"]["z"] = rotation.z;

    data["Size"]["x"] = m_size.x;
    data["Size"]["y"] = m_size.y;
    data["Size"]["z"] = m_size.z;

    data["Ambient"]["x"] = m_material.ambient.x;
    data["Ambient"]["y"] = m_material.ambient.y;
    data["Ambient"]["z"] = m_material.ambient.z;
    data["Ambient"]["w"] = m_material.ambient.w;

    data["Diffuse"]["x"] = m_material.diffuse.x;
    data["Diffuse"]["y"] = m_material.diffuse.y;
    data["Diffuse"]["z"] = m_material.diffuse.z;
    data["Diffuse"]["w"] = m_material.diffuse.w;

    data["Specular"]["x"] = m_material.specular.x;
    data["Specular"]["y"] = m_material.specular.y;
    data["Specular"]["z"] = m_material.specular.z;
    data["Specular"]["w"] = m_material.specular.w;

    data["Emissive"]["x"] = m_material.emissive.x;
    data["Emissive"]["y"] = m_material.emissive.y;
    data["Emissive"]["z"] = m_material.emissive.z;
    data["Emissive"]["w"] = m_material.emissive.w;

    data["SpecularPower"] = m_material.specularPower;

    data["children"] = Json::array();

    return data;
}

void SceneNode::SetPosition(const glm::vec3& pos)
{
    m_transform[3][0] = pos.x;
    m_transform[3][1] = pos.y;
    m_transform[3][2] = pos.z;
}

glm::vec3 SceneNode::GetPosition()
{
    glm::vec3 vec;
    vec.x = m_transform[3][0];
    vec.y = m_transform[3][1];
    vec.z = m_transform[3][2];

    return vec;
}

//-----------------------------------------------------------------------------
// PawnNode Implementation
//-----------------------------------------------------------------------------

const char* PawnNode::g_type = "PawnNode";

PawnNode::PawnNode()
{

}

PawnNode::~PawnNode()
{

}

void PawnNode::Init(const Json& data)
{
    SceneNode::Init(data);
}

void PawnNode::PostInit()
{
    SceneNode::PostInit();
}

void PawnNode::Update(const float deltaTime)
{
    SceneNode::Update(deltaTime);
}

void PawnNode::Render()
{

}

Json PawnNode::ToJSON()
{
    Json data = SceneNode::ToJSON();

    return data;
}

//-----------------------------------------------------------------------------
// TestNode Implementation
//-----------------------------------------------------------------------------

const char* TestNode::g_type = "TestNode";

TestNode::TestNode()
    : SceneNode()
{
    m_testElement = glm::vec3(0, 0, 0);

    m_numVerts = 0;
}

TestNode::~TestNode()
{

}

void TestNode::Init(const Json& data)
{
    SceneNode::Init(data);
}

void TestNode::PostInit()
{
    SceneNode::PostInit();

    m_numVerts = 3;

    std::vector<Vertex_LitTexturedColored> verts;
    verts.resize(m_numVerts);

    verts[0].pos = glm::vec3( 0.0f, 0.5f, 0.0f); verts[0].norm = glm::vec3(0, 1, 0); verts[0].color = glm::vec3(1, 0, 0); verts[0].uv = glm::vec2(0, 0);
    verts[1].pos = glm::vec3(-0.5f,-0.5f, 0.0f); verts[1].norm = glm::vec3(0, 1, 0); verts[1].color = glm::vec3(0, 1, 0); verts[1].uv = glm::vec2(0, 0); 
    verts[2].pos = glm::vec3( 0.5f,-0.5f, 0.0f); verts[2].norm = glm::vec3(0, 1, 0); verts[2].color = glm::vec3(0, 0, 1); verts[2].uv = glm::vec2(0, 0); 

    m_vertexArray.Init();
    m_vertexArray.SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_vertexArray.SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_vertexArray.SetVertexAttribute(0, 2, 3, GL_FLOAT, 24);
    m_vertexArray.SetVertexAttribute(0, 3, 2, GL_FLOAT, 36);

    long long vertBufferSize = sizeof(Vertex_LitTexturedColored) * verts.size();

    m_vertexBuffer.Init(vertBufferSize, GL_STATIC_DRAW);
    m_vertexBuffer.MapMemory(0, vertBufferSize, verts.data());

    m_vertexArray.SetVertexBuffer(0, &m_vertexBuffer, sizeof(Vertex_LitTexturedColored), VertexArrayInputRate_Vertex);
    
    verts.clear();
}

void PrintMatrix(const glm::mat4& matrix) 
{
    std::cout << "Mat4x4\n";

    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            std::cout << matrix[j][i] << " ";
        }

        std::cout << '\n';
    }
}

static float timer = 0;

void TestNode::Update(const float deltaTime)
{
    SceneNode::Update(deltaTime);

    g_pApp->GetRenderer()->AddBox(GetPosition(), glm::vec3(1, 1, 1), glm::vec3(1, 0, 1));

    timer += deltaTime;

    if (timer >= 1.0f)
    {
        timer = 0.0f;

        // PrintMatrix(m_transform);
    }
}

void TestNode::Render()
{
    Shader* pShader = g_pApp->GetRenderer()->GetShader_LitTexturedColored();

    pShader->SetUniformMatrix4f("uModel", m_transform);

    pShader->SetUniform4f("uMat.ambient", m_material.ambient);
    pShader->SetUniform4f("uMat.diffuse", m_material.diffuse);
    pShader->SetUniform4f("uMat.specular", m_material.specular);
    pShader->SetUniform4f("uMat.emissive", m_material.emissive);
    pShader->SetUniform1f("uMat.specularPower", m_material.specularPower);
    pShader->SetUniform1b("uMat.bUseTex", m_material.bUseTexture);

    m_vertexArray.Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
}

Json TestNode::ToJSON()
{
    Json data = SceneNode::ToJSON();

    return data;
}

//-----------------------------------------------------------------------------
// MeshNode Implementation
//-----------------------------------------------------------------------------

const char* MeshNode::g_type = "MeshNode";

MeshNode::MeshNode()
    : SceneNode()
{

}

MeshNode::~MeshNode()
{

}

void MeshNode::Init(const Json& data)       
{
    SceneNode::Init(data);

    if (data.contains("MeshAsset"))
    {
        m_meshAsset = data["MeshAsset"].get<std::string>();
    }

    if (data.contains("TextureAsset"))
    {
        m_textureAsset = data["TextureAsset"].get<std::string>();
    }
}

glm::vec3 aabbPos; // = mesh->GetBoundingBoxCenter() + GetPosition();
glm::vec3 aabbExtents; // = mesh->GetBoundingBoxExtents();

glm::vec3 updatedAabbPos;
glm::vec3 updatedAabbExtents;

void MeshNode::PostInit()                   
{
    SceneNode::PostInit();

    // std::shared_ptr<Mesh> mesh = g_pApp->GetMesh(m_meshAsset);
    
    // aabbPos = mesh->GetBoundingBoxCenter();
    
    // aabbExtents = mesh->GetBoundingBoxExtents();
    // aabbExtents *= 2.0f;

    // updatedAabbPos = aabbPos;
    // updatedAabbExtents = aabbExtents;
}

void MeshNode::Update(const float deltaTime)
{
    SceneNode::Update(deltaTime);

    m_material.bUseTexture = (!m_textureAsset.empty()) ? true : false;
}

void MeshNode::Render()                     
{
    Shader* pShader = g_pApp->GetRenderer()->GetShader_LitTexturedColored();

    pShader->SetUniformMatrix4f("uModel", m_transform);

    pShader->SetUniform4f("uMat.ambient", m_material.ambient);
    pShader->SetUniform4f("uMat.diffuse", m_material.diffuse);
    pShader->SetUniform4f("uMat.specular", m_material.specular);
    pShader->SetUniform4f("uMat.emissive", m_material.emissive);
    pShader->SetUniform1f("uMat.specularPower", m_material.specularPower);
    pShader->SetUniform1b("uMat.bUseTex", m_material.bUseTexture);

    if (m_material.bUseTexture)
    {
        pShader->SetUniform1i("uTex", 0);
        std::shared_ptr<Texture> tex = g_pApp->GetTexture(m_textureAsset);
        tex->BindUnit(0);
    }

    std::shared_ptr<Mesh> mesh = g_pApp->GetMesh(m_meshAsset);
    mesh->Render();

    // aabbPos = mesh->GetBoundingBoxCenter(); // + GetPosition();
    // aabbExtents = mesh->GetBoundingBoxExtents();

    /*
    if (m_bChanged)
    {
        glm::mat3 m(glm::toMat3(m_rotation));
        glm::vec3 t(aabbPos);

        for (int i = 0; i < 3; i++) 
        {
            updatedAabbPos[i] = t[i];
            updatedAabbExtents[i] = 0.0f;

            for (int j = 0; j < 3; j++) 
            {
                updatedAabbPos[i] += m[i][j] * aabbPos[j];
                updatedAabbExtents[i] += glm::abs(m[i][j]) * aabbExtents[j];
            }
        }
    
        // aabbPos = updatedAabbPos;
        // aabbExtents = updatedAabbExtents;

        m_bChanged = false;
    }
    */

    // glm::vec3 extents = mesh->GetBoundingBoxExtents();
    // extents.x = glm::abs(extents.x);
    // extents.y = glm::abs(extents.y);
    // extents.z = glm::abs(extents.z);

    // m_boundingSphereRadius = 0.0f;
    // m_boundingSphereRadius = (m_boundingSphereRadius > extents.x) ? m_boundingSphereRadius : extents.x;
    // m_boundingSphereRadius = (m_boundingSphereRadius > extents.y) ? m_boundingSphereRadius : extents.y;
    // m_boundingSphereRadius = (m_boundingSphereRadius > extents.z) ? m_boundingSphereRadius : extents.z;

    // glm::vec3 boundingSpherePosition = mesh->GetBoundingBoxCenter() + GetPosition();

    // g_pApp->GetRenderer()->AddSphere(boundingSpherePosition, m_boundingSphereRadius, glm::vec3(1, 0, 1));

    // g_pApp->GetRenderer()->AddBox(GetPosition() + aabbPos, aabbExtents, glm::vec3(1, 1, 1));
    // g_pApp->GetRenderer()->AddBox(GetPosition() + updatedAabbPos, updatedAabbExtents, glm::vec3(1, 1, 1));
}

Json MeshNode::ToJSON()
{
    Json data = SceneNode::ToJSON();

    data["MeshAsset"] = m_meshAsset;
    data["TextureAsset"] = m_textureAsset;

    return data;
}

//-----------------------------------------------------------------------------
// LightNode Implementation
//-----------------------------------------------------------------------------

const char* LightNode::g_type = "LightNode";
    
LightNode::LightNode()
    : SceneNode()
{

}

LightNode::~LightNode()
{

}

void LightNode::Init(const Json& data)        
{
    SceneNode::Init(data);

    if (data.contains("LightType"))
    {
        m_lightProperties.type = data["LightType"].get<int>();
    }

    if (data.contains("LightPosition"))
    {
        float x = 1;
        float y = 1;
        float z = 1;

        if (data["LightPosition"].contains("x") 
        && data["LightPosition"].contains("y") 
        && data["LightPosition"].contains("z")) 
        {
            x = data["LightPosition"]["x"].get<float>();
            y = data["LightPosition"]["y"].get<float>();
            z = data["LightPosition"]["z"].get<float>();
        }  

        m_lightProperties.position = glm::vec3(x, y, z);
    }

    if (data.contains("Direction"))
    {
        float x = 1;
        float y = 1;
        float z = 1;

        if (data["Direction"].contains("x") 
        && data["Direction"].contains("y") 
        && data["Direction"].contains("z")) 
        {
            x = data["Direction"]["x"].get<float>();
            y = data["Direction"]["y"].get<float>();
            z = data["Direction"]["z"].get<float>();
        }  

        m_lightProperties.direction = glm::vec3(x, y, z);
    }

    if (data.contains("Range"))
    {
        m_lightProperties.range = data["Range"].get<float>();
    }

    if (data.contains("FallOff"))
    {
        m_lightProperties.fallOff = data["FallOff"].get<float>();
    }

    if (data.contains("ConstantAttenuation"))
    {
        m_lightProperties.constantAttenuation = data["ConstantAttenuation"].get<float>();
    }

    if (data.contains("LinearAttenuation"))
    {
        m_lightProperties.linearAttenuation = data["LinearAttenuation"].get<float>();
    }

    if (data.contains("QuadraticAttenuation"))
    {
        m_lightProperties.quadraticAttenuation = data["QuadraticAttenuation"].get<float>();
    }

    if (data.contains("Theta"))
    {
        m_lightProperties.theta = data["Theta"].get<float>();
    }

    if (data.contains("Phi"))
    {
        m_lightProperties.phi = data["Phi"].get<float>();
    }
}

void LightNode::PostInit()                    
{
    SceneNode::PostInit();
}

void LightNode::Update(const float deltaTime) 
{
    SceneNode::Update(deltaTime);

    glm::vec3 position = glm::vec3(m_transform[3][0], m_transform[3][1], m_transform[3][2]);

    g_pApp->GetRenderer()->AddCross(position, 0.1f, glm::vec3(1, 0, 0));
    
    if (m_lightProperties.type == LightType_Directional || m_lightProperties.type == LightType_Spot)
    {
        g_pApp->GetRenderer()->AddLine(position, (position + m_lightProperties.direction), glm::vec3(1, 0, 0));
    }

    m_lightProperties.position = position;
}

void LightNode::Render()                      
{

}

Json LightNode::ToJSON()
{
    Json data = SceneNode::ToJSON();

    data["LightType"] = m_lightProperties.type;
    
    data["LightPosition"]["x"] = m_position.x;
    data["LightPosition"]["y"] = m_position.y;
    data["LightPosition"]["z"] = m_position.z;

    data["Direction"]["x"] = m_lightProperties.direction.x;
    data["Direction"]["y"] = m_lightProperties.direction.y;
    data["Direction"]["z"] = m_lightProperties.direction.z;

    data["Range"] = m_lightProperties.range;

    data["FallOff"] = m_lightProperties.fallOff;

    data["ConstantAttenuation"] = m_lightProperties.constantAttenuation;
    data["LinearAttenuation"] = m_lightProperties.linearAttenuation;
    data["QuadraticAttenuation"] = m_lightProperties.quadraticAttenuation;

    data["Theta"] = m_lightProperties.theta;
    data["Phi"] = m_lightProperties.phi;

    return data;
}

/*
//-----------------------------------------------------------------------------
// SceneNode Implementation
//-----------------------------------------------------------------------------

SceneNode::SceneNode()
{
    m_pParent = nullptr;

    m_name = "SceneNode";

    // m_bVisible = true;

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
//  CubeMapNode Implementation
////////////////////////////////////////////////////

CubeMapNode::CubeMapNode()
{
    m_CubeMapSideVertCount = 6;

    std::vector<Vertex> vertices =
    {
        {{ 0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }},
        {{ 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ 0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},

        {{ 0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},
        {{ 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ 0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }},

        {{ -0.5f,  0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ -0.5f,  0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},

        {{ -0.5f,  0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ -0.5f, -0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }},

        {{  0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }},
        {{  0.5f, 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }},

        {{  0.5f, 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ -0.5f, 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }},

        {{  0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }},
        {{  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }},

        {{  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }},
        {{ -0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }},

        {{ -0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }},
        {{ -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
        {{  0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},

        {{  0.5f,  0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
        {{ -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
        {{  0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }},

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
    // m_VertexBuffer.reset(new VertexBuffer(vertexBufferSize, GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, vertexBufferSize, vertices.data());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    m_numVerts = vertices.size();
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

void CubeMapNode::Init(Scene& scene)
{
    SceneNode::Init(scene);

    m_shader.Load("Assets/Shaders/Sky.vert", "Assets/Shaders/Sky.frag");
    m_shader.SetUniformBlockBinding(0, "Matrices");
}

void CubeMapNode::Render(Scene& scene)
{
    glDepthFunc(GL_LEQUAL);

    m_shader.Bind();
    m_shader.SetUniformMatrix4f("u_WorldView", glm::mat4(glm::mat3(scene.GetCamera()->GetView())));
    m_shader.SetUniformMatrix4f("u_WorldProjection", scene.GetCamera()->GetProjection());

    m_VertexArray->Bind();

    for (uint32_t i = 0; i < CubeMapSide_Count; i++)
    {
        const uint32_t texUnit = 0;
        m_Textures[i]->BindUnit(texUnit);
        m_shader.SetUniform1i("u_Texture", texUnit);

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
    m_shader.Load("Assets/Shaders/Billboard.vert", "Assets/Shaders/Billboard.frag");
    m_shader.SetUniformBlockBinding(0, "Matrices");

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
    // m_VertexBuffer.reset(new VertexBuffer(vertexBufferSize, GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, vertexBufferSize, vertices.data());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    vertices.clear();

    const signed long long indexBufferSize = sizeof(uint32_t) * indices.size();
    // m_IndexBuffer.reset(new IndexBuffer(indexBufferSize, GL_STATIC_DRAW));
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
    m_shader.Bind();

    if (m_material.bUseTexture)
    {
        const uint32_t texUnit = 0;
        m_Texture->BindUnit(texUnit);
        m_shader.SetUniform1i("u_Texture", texUnit);
    }

    m_pos = m_pParent->GetPosition();
    m_shader.SetUniform3f("u_Position", m_pos);
    m_shader.SetUniform1f("scale", scale);

    m_VertexArray->Bind();
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}

////////////////////////////////////////////////////
//  TerrainNode Implementation
////////////////////////////////////////////////////

TerrainNode::TerrainNode()
{
    m_shader.Load("Assets/Shaders/Terrain.vert", "Assets/Shaders/Terrain.frag");
    m_shader.SetUniformBlockBinding(0, "Matrices");
    m_shader.SetUniformBlockBinding(1, "Lighting");
    
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
    // m_VertexBuffer.reset(new VertexBuffer(vertexBufferSize, GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, vertexBufferSize, vertices.data());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    vertices.clear();

    const signed long long indexBufferSize = sizeof(uint32_t) * indices.size();
    // m_IndexBuffer.reset(new IndexBuffer(indexBufferSize, GL_STATIC_DRAW));
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
    m_shader.Bind();

    m_shader.SetUniformMatrix4f("u_World", glm::mat4(1.0f));

    m_shader.SetUniform4f("u_Material.Ambient", m_material.Ambient);
    m_shader.SetUniform4f("u_Material.Diffuse", m_material.Diffuse);
    m_shader.SetUniform4f("u_Material.Specular", m_material.Specular);
    m_shader.SetUniform4f("u_Material.Emissive", m_material.Emissive);
    m_shader.SetUniform1f("u_Material.SpecularPower", m_material.SpecularPower);
    m_shader.SetUniform1b("u_Material.bUseTexture", m_material.bUseTexture);

    m_material.bUseTexture = true;
    if (m_material.bUseTexture)
    {
        scene.GetTexture("Assets/Heightmaps/BlendMap.png")->BindUnit(0);
        m_shader.SetUniform1i("u_BlendMapTexture", 0);

        scene.GetTexture("Assets/Textures/Terrain/ElwynnDirtBase.png")->BindUnit(1);
        m_shader.SetUniform1i("u_DirtBaseTexture", 1);

        scene.GetTexture("Assets/Textures/Terrain/ElwynnCobblestoneBase.png")->BindUnit(2);
        m_shader.SetUniform1i("u_StonebrickTexture", 2);

        scene.GetTexture("Assets/Textures/Terrain/ElwynnGrassBase.png")->BindUnit(3);
        m_shader.SetUniform1i("u_GrassTexture", 3);

        scene.GetTexture("Assets/Textures/Terrain/ElwynnCrop.png")->BindUnit(4);
        m_shader.SetUniform1i("u_CropTex", 4);
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
    m_lamp.reset(new TestNode(scene.GetMesh("Assets/Models/Cube.obj"), g_pShader_LitTextured, scene.GetTexture("Assets/Textures/UvGrid.png")));
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

*/