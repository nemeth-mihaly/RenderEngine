#include "Actor.h"

#include <cstdio>

#include "3rdParty/KHR/khrplatform.h"
#include "3rdParty/glad/glad.h"

//-----------------------------------------------------------------------------
// Actor Implementation
//-----------------------------------------------------------------------------

Actor::Actor(ActorId id)
    : m_Id(id)
{
    printf("Creating Actor\n");
}

Actor::~Actor()
{
    printf("Destroying Actor\n");
}

void Actor::Init()
{
    AddComponent(std::make_shared<TransformComponent>());

    m_Mesh = std::make_shared<Mesh>();
    m_Mesh->Init();
    
    auto meshDrawComponent = std::make_shared<MeshDrawComponent>();
    meshDrawComponent->SetMesh(m_Mesh);

    AddComponent(meshDrawComponent);
}

void Actor::AddComponent(std::shared_ptr<ActorComponent> component)
{
    auto findItr = m_Components.find(component->GetType());
    if (findItr != m_Components.end()) { return; }

    m_Components[component->GetType()] = component;
}

std::weak_ptr<TransformComponent> Actor::GetTransformComponent()
{
    auto findItr = m_Components.find(TransformComponent::s_Type);
    if (findItr != m_Components.end()) 
    { 
        std::shared_ptr<TransformComponent> component = std::static_pointer_cast<TransformComponent>(findItr->second);
        return std::weak_ptr<TransformComponent>(component); 
    }

    return std::weak_ptr<TransformComponent>();
}

std::weak_ptr<MeshDrawComponent> Actor::GetMeshDrawComponent()
{
    auto findItr = m_Components.find(MeshDrawComponent::s_Type);
    if (findItr != m_Components.end()) 
    { 
        std::shared_ptr<MeshDrawComponent> component = std::static_pointer_cast<MeshDrawComponent>(findItr->second);
        return std::weak_ptr<MeshDrawComponent>(component); 
    }

    return std::weak_ptr<MeshDrawComponent>();
}