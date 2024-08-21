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

private:
    int             m_NumVerts;
    uint32_t        m_VaoId;
    uint32_t        m_VboId;
};