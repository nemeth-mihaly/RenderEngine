#include "Graphics3D/SceneNodes.h"

////////////////////////////////////////////////////
//  SceneNode Implementation
////////////////////////////////////////////////////

SceneNode::SceneNode()
{
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

void SceneNode::VRender(StrongShaderProgPtr& shader)
{
}

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

MeshNode::MeshNode(const StrongVertexArrayPtr& vertexArray, const StrongMeshPtr& mesh, const StrongTexturePtr& texture)
    : m_VertexArray(vertexArray), m_Mesh(mesh), m_Texture(texture)
{
}

MeshNode::~MeshNode()
{
}

void MeshNode::VCreate()
{
}

void MeshNode::VRender(StrongShaderProgPtr& shader)
{
    if (m_Texture)
    {
        m_Texture->BindUnit(0);
        shader->SetUniform1b("u_bHasTexture", true); 
    }
    else
    {
        shader->SetUniform1b("u_bHasTexture", false); 
    }

    glm::mat4 world = glm::translate(glm::mat4(1.0f), m_Pos);
    shader->SetUniformMatrix4f("u_World", world);

    m_VertexArray->Bind();

    glDrawArrays(GL_TRIANGLES, m_Mesh->VertexBufferOffset, m_Mesh->VertexCount);
}