#pragma once

#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

#include "Mesh.h"

class Actor;

typedef uint32_t ComponentType;

//-----------------------------------------------------------------------------
// class Component
//-----------------------------------------------------------------------------

class Component
{
public:
    virtual ~Component() { printf("Destroying Component\n"); }
    virtual ComponentType GetType() const = 0;

    void SetOwner(std::weak_ptr<Actor> owner) { m_Owner = owner; }
    std::weak_ptr<Actor> GetOwner() { return m_Owner; }

private:
    std::weak_ptr<Actor> m_Owner;
};

//-----------------------------------------------------------------------------
// class TransformComponent
//-----------------------------------------------------------------------------

class TransformComponent : public Component
{
public:
    static constexpr ComponentType s_Type = 0x348BF9B5;

    TransformComponent();
    virtual ComponentType GetType() const { return s_Type; }

    void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
    const glm::vec3& GetPosition() const { return m_Pos; }

private:
    glm::vec3       m_Pos;
};

//-----------------------------------------------------------------------------
// class MeshDrawComponent
//-----------------------------------------------------------------------------

class MeshDrawComponent : public Component
{
public:
    static constexpr ComponentType s_Type = 0x548592CB;

    MeshDrawComponent();
    virtual ComponentType GetType() const { return s_Type; }

    void SetMeshAsset(const std::string& asset) { m_MeshAsset = asset; }
    const std::string& GetMeshAsset() const { return m_MeshAsset; }

private:
    std::string             m_MeshAsset;
};