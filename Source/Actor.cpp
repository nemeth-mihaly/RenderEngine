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

}

void Actor::AddComponent(std::shared_ptr<Component> component)
{
    auto findItr = m_Components.find(component->GetType());
    if (findItr != m_Components.end()) { return; }

    m_Components[component->GetType()] = component;
}