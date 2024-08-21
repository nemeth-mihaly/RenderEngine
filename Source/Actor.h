#pragma once

#include <cstdint>
#include <vector>

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

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
    void Draw();

    ActorId GetId() const { return m_Id; }

    void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
    const glm::vec3& GetPosition() const { return m_Pos; }

private:
    ActorId         m_Id;

    glm::vec3       m_Pos;

    int             m_NumVerts;
    uint32_t        m_VaoId;
    uint32_t        m_VboId;
};