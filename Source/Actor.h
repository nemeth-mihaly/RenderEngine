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
    
    template<class T>
    std::weak_ptr<T> GetComponent()
    {
        auto findItr = m_Components.find(T::s_Type);
        if (findItr != m_Components.end()) 
        { 
            std::shared_ptr<T> component = std::static_pointer_cast<T>(findItr->second);
            return std::weak_ptr<T>(component); 
        }

        return std::weak_ptr<T>();
    }

private:
    ActorId         m_Id;
    std::unordered_map<ActorComponentType, std::shared_ptr<ActorComponent>> m_Components;

    std::shared_ptr<Mesh> m_Mesh;
};