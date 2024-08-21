#pragma once

#include <cstdint>
#include <vector>

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"

//-----------------------------------------------------------------------------
// class Actor
//-----------------------------------------------------------------------------

class Actor
{
public:
    Actor();
    ~Actor();

    void Init();
    void Draw();

    void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
    const glm::vec3& GetPosition() const { return m_Pos; }

private:
    glm::vec3       m_Pos;

    int             m_NumVerts;
    uint32_t        m_VaoId;
    uint32_t        m_VboId;
};