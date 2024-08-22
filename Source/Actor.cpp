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