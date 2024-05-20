#include "Graphics3D/SceneNodes.h"
#include "Core/Application.h"

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

void SceneNode::VCreate()
{
}

void SceneNode::VUpdate(const float deltaTime)
{
}

void SceneNode::VRender()
{
}

#if 0
void SceneNode::VRender(StrongShaderProgPtr& shader)
{
}
#endif

////////////////////////////////////////////////////
//  SceneNode Implementation
////////////////////////////////////////////////////

CameraNode::CameraNode()
{
    m_ForwardDir = glm::vec3(0.0f, 0.0f, -1.0f);
}

CameraNode::~CameraNode()
{
}

void CameraNode::VCreate()
{
    m_Projection = glm::perspective(glm::radians(45.0f), (1280 / static_cast<float>(720)), 0.001f, 1000.0f);
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
    : m_MeshName(meshName), m_ShaderProgName(shaderProgName), m_TextureName(textureName)
{
}


MeshNode::~MeshNode()
{
}

void MeshNode::VCreate()
{
}

void MeshNode::VRender()
{
    StrongShaderProgPtr shaderProgram = g_pApp->GetAssetManager().GetShaderProgram(m_ShaderProgName);

    shaderProgram->Use();

    shaderProgram->SetUniform4f("u_Material.Ambient", m_Material.Ambient);
    shaderProgram->SetUniform4f("u_Material.Diffuse", m_Material.Diffuse);
    shaderProgram->SetUniform4f("u_Material.Specular", m_Material.Specular);
    shaderProgram->SetUniform4f("u_Material.Emissive", m_Material.Emissive);
    shaderProgram->SetUniform1f("u_Material.SpecularPower", m_Material.SpecularPower);
    shaderProgram->SetUniform1b("u_Material.bUseTexture", m_Material.bUseTexture);

    if (m_Material.bUseTexture)
    {
        StrongTexturePtr texture = g_pApp->GetAssetManager().GetTexture(m_TextureName);
        //texture->BindUnit(0);
        texture->SetActiveUnit(0);
        shaderProgram->SetUniform1i("u_Texture", 0);
    }

    glm::mat4 world = glm::translate(glm::mat4(1.0f), m_Pos);
    world *= glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.w), glm::vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z));
    world *= glm::scale(glm::mat4(1.0f), m_Scale);

    shaderProgram->SetUniformMatrix4f("u_World", world);

    StrongMeshPtr mesh = g_pApp->GetAssetManager().GetMesh(m_MeshName);

    mesh->m_VertexArray->Bind();
    glDrawArrays(GL_TRIANGLES, mesh->VertexBufferOffset, mesh->VertexCount);
}

////////////////////////////////////////////////////
//  MeshNode Implementation
////////////////////////////////////////////////////

LightNode::LightNode()
{
}

LightNode::~LightNode()
{
}

void LightNode::VCreate()
{
}

void LightNode::VUpdate(const float deltaTime)
{
}