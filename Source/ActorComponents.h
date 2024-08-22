#pragma once

#include <cstdint>
#include <cstdio>
#include <memory>

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

#include "Mesh.h"

typedef uint32_t ActorComponentType;

//-----------------------------------------------------------------------------
// class ActorComponent
//-----------------------------------------------------------------------------

class ActorComponent
{
public:
    virtual ~ActorComponent() { printf("Destroying ActorComponent\n"); }
    virtual ActorComponentType GetType() const = 0;
};

//-----------------------------------------------------------------------------
// class TransformComponent
//-----------------------------------------------------------------------------

class TransformComponent : public ActorComponent
{
public:
    static constexpr ActorComponentType s_Type = 0x348BF9B5;

    TransformComponent();
    virtual ActorComponentType GetType() const { return s_Type; }

    void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
    const glm::vec3& GetPosition() const { return m_Pos; }

private:
    glm::vec3       m_Pos;
};

//-----------------------------------------------------------------------------
// class MeshDrawComponent
//-----------------------------------------------------------------------------

class MeshDrawComponent : public ActorComponent
{
public:
    static constexpr ActorComponentType s_Type = 0x548592CB;

    MeshDrawComponent();
    virtual ActorComponentType GetType() const { return s_Type; }

    void SetMesh(std::shared_ptr<Mesh> mesh) { m_Mesh = mesh; }
    std::shared_ptr<Mesh> GetMesh() { return m_Mesh; }

private:
    std::shared_ptr<Mesh> m_Mesh;
};