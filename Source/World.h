#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>

#include "Geometry.h"

#include "VertexArray.h"
#include "Buffers.h"
#include "Texture.h"

//-----------------------------------------------------------------------------
// class World
//-----------------------------------------------------------------------------

class World
{
 public:
    World();
    ~World();

    void Init();
    void Update(float deltaTime);
    void Render();

    bool HeightAt(const glm::vec3& pos, float& _out_height);
    bool Raycast(const Ray& ray, float& _out_t);

    std::vector<Vertex_UnlitTexturedColored*> GetVertsSelectedByBrush(const glm::vec3& pos, float radius);
    
    void SaveHeightmap();

    bool            m_bStagingVertBufferChanged;

 private:
    glm::ivec3      m_extents;

    bool            m_bFlushStagingVertBufferAfterInit;
    std::vector<Vertex_UnlitTexturedColored>    m_verts;

    int                 m_numInds;

    VertexArray         m_vertArray;
    VertexBuffer        m_vertBuffer;
    IndexBuffer         m_indBuffer;

    Texture*    m_pBlendTex;
    uint8_t*    m_pBlendTexData;
};