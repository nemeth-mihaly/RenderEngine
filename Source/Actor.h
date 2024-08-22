#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

#include "ActorComponents.h"

#include "Mesh.h"

typedef uint32_t ActorId;

//-----------------------------------------------------------------------------
// class Actor
//-----------------------------------------------------------------------------

class Actor
{
public:
    Actor(ActorId id);
    ~Actor();

    void Init();
    
    ActorId GetId() const { return m_Id; }

    void AddComponent(std::shared_ptr<ActorComponent> component);

    std::weak_ptr<TransformComponent> GetTransformComponent();
    std::weak_ptr<MeshDrawComponent> GetMeshDrawComponent();

private:
    ActorId         m_Id;
    std::unordered_map<ActorComponentType, std::shared_ptr<ActorComponent>> m_Components;

    std::shared_ptr<Mesh> m_Mesh;
};