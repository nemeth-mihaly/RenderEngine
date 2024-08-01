#include "World.h"

#include <stdio.h>
#include <iostream>

#include "3rdParty/stb/stb_image.h"
#include "3rdParty/stb/stb_image_write.h"

#include "Application.h"

static void Barycentric()
{

}

//-----------------------------------------------------------------------------
// World Implementation
//-----------------------------------------------------------------------------

World::World()
{
    m_extents = glm::ivec3(32, 0, 32);

    m_bFlushStagingVertBufferAfterInit = false;
    m_bStagingVertBufferChanged = false;

    m_numInds = 0;
}

World::~World()
{

}

void World::Init()
{
    stbi_set_flip_vertically_on_load(0);

    int numHeightmapChannels;
    uint8_t* pHeightmapData = stbi_load("Assets/Textures/Heightmap32x32.png", &m_extents.x, &m_extents.z, &numHeightmapChannels, 0);

    m_verts.reserve(m_extents.x * m_extents.z);

    for (int z = 0; z < m_extents.z; z++)
    {
        for (int x = 0; x < m_extents.x; x++)
        {
            long long heightMapDataOffset = (x + z * m_extents.z) * numHeightmapChannels;
            uint8_t colorR = pHeightmapData[heightMapDataOffset + 0];
            uint8_t colorG = pHeightmapData[heightMapDataOffset + 1];
            uint8_t colorB = pHeightmapData[heightMapDataOffset + 2];

            uint32_t color = 0;
            color =
                                ((colorR & 0xff) << 16) | 
                                ((colorG & 0xff) << 8 ) | 
                                ((colorB & 0xff));

            // uint32_t mgc = 16'777'216;
            // uint32_t mgc = 0xffffffff;
            // float y = color / (float)mgc;
            float y = colorR / 255.0f;
            y *= 64.0f;
            y -= 32.0f;

            Vertex_UnlitTexturedColored vert;

            vert.pos = glm::vec3(0, 0, 0);
            // vert.pos.x = -m_extents.x / 2.0f + x;
            // vert.pos.z = -m_extents.z / 2.0f + z;
            vert.pos.x = x;
            vert.pos.y = y;
            vert.pos.z = z;

            vert.color = glm::vec3(1, 1, 1);

            vert.uv.x = static_cast<float>(x) / static_cast<float>(m_extents.x - 1);
            vert.uv.y = static_cast<float>(z) / static_cast<float>(m_extents.z - 1);;

            m_verts.push_back(vert);
        }
    }

    stbi_image_free(pHeightmapData);

    std::vector<uint32_t> inds;

    for (int z = 0; z < (m_extents.z - 1); z++)
    {
        for (int x = 0; x < (m_extents.x - 1); x++)
        {            
            inds.push_back((z + 0) * m_extents.x + x + 0);
            inds.push_back((z + 1) * m_extents.x + x + 0);
            inds.push_back((z + 0) * m_extents.x + x + 1);
            
            inds.push_back((z + 0) * m_extents.x + x + 1);
            inds.push_back((z + 1) * m_extents.x + x + 0);
            inds.push_back((z + 1) * m_extents.x + x + 1);
        }
    }

    m_vertArray.Init();
    m_vertArray.SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_vertArray.SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_vertArray.SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);

    GLenum vertBufferUsage = (m_bFlushStagingVertBufferAfterInit != true ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

    m_vertBuffer.Init(sizeof(Vertex_UnlitTexturedColored) * m_verts.size(), vertBufferUsage);
    m_vertBuffer.MapMemory(0, sizeof(Vertex_UnlitTexturedColored) * m_verts.size(), m_verts.data());
    
    m_vertArray.SetVertexBuffer(0, &m_vertBuffer, sizeof(Vertex_UnlitTexturedColored), VertexArrayInputRate_Vertex);

    m_indBuffer.Init(sizeof(uint32_t) * inds.size(), GL_STATIC_DRAW);
    m_indBuffer.MapMemory(0, sizeof(uint32_t) * inds.size(), inds.data());
    
    m_vertArray.SetIndexBuffer(&m_indBuffer);

    if (m_bFlushStagingVertBufferAfterInit)
    {
        m_verts.clear();
    }

    m_numInds = inds.size();
    inds.clear();
}

/* 
    for (int i = 0; i < m_HeightMapHeight; i++)
    {
        for (int j = 0; j < m_HeightMapWidth; j++)
        {
            float hL = HeightAt((j - 1 + m_HeightMapHeight) % m_HeightMapHeight, i    );
            float hR = HeightAt((j + 1 + m_HeightMapHeight) % m_HeightMapHeight, i    );
            float hD = HeightAt(j    , (i - 1 + m_HeightMapHeight) % m_HeightMapHeight);
            float hU = HeightAt(j    , (i + 1 + m_HeightMapHeight) % m_HeightMapHeight);

            vertices[i * m_HeightMapWidth + j].Normal = glm::normalize(glm::vec3(hL - hR, dim * 4.0f, hD - hU));
        }
    }
*/

void World::Update(float deltaTime)
{

}

void World::Render()
{
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->Bind();

    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->SetUniformMatrix4f("uView", g_pApp->GetRenderer()->GetCamera()->GetView());
    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->SetUniformMatrix4f("uProjection", g_pApp->GetRenderer()->GetCamera()->GetProjection());

    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->SetUniformMatrix4f("uModel", glm::mat4(1));

    glm::vec3 brushPos = g_pApp->GetBrushPos();
    float brushRadius = g_pApp->GetBrushRadius();

    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->SetUniform3f("uBrushPos", brushPos);
    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->SetUniform1f("uBrushRadius", brushRadius);

    g_pApp->GetTexture("Assets/Textures/Blendmap32x32.png")->BindUnit(0);
    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->SetUniform1i("uBlendmapTex", 0);

    g_pApp->GetTexture("Assets/Textures/mullgorebasedirt.png")->BindUnit(1);
    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->SetUniform1i("uTilesetTex1", 1);

    g_pApp->GetTexture("Assets/Textures/mullgorebasegrass.png")->BindUnit(2);
    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->SetUniform1i("uTilesetTex2", 2);

    g_pApp->GetTexture("Assets/Textures/mullgoreroad01.png")->BindUnit(3);
    g_pApp->GetRenderer()->GetShader_UnlitTexturedColoredTerrain()->SetUniform1i("uTilesetTex3", 3);

    m_vertArray.Bind();

    if ((!m_bFlushStagingVertBufferAfterInit) && m_bStagingVertBufferChanged)
    {
        m_bStagingVertBufferChanged = false;
        m_vertBuffer.MapMemory(0, sizeof(Vertex_UnlitTexturedColored) * m_verts.size(), m_verts.data());
    }

    glDrawElements(GL_TRIANGLES, m_numInds, GL_UNSIGNED_INT, nullptr);

    if (!m_bFlushStagingVertBufferAfterInit)
    {
        /*       
        for (int z = 0; z < m_extents.z; z++) 
        {
            for (int x = 0; x < m_extents.x; x++)
            {
                int pv1 = x + z * m_extents.z;
                glm::vec3 v1 = m_verts[pv1].pos;
                v1.y += 0.05f;

                if (x < m_extents.x - 1)
                {
                    int pv2 = (x + 1) + z * m_extents.z;
                    glm::vec3 v2 = m_verts[pv2].pos;
                    v2.y += 0.05f;

                    g_pApp->GetRenderer()->AddLine(v1, v2, glm::vec3(1, 1, 1));
                }

                if (z < m_extents.z - 1)
                {
                    int pv3 = x + (z + 1) * m_extents.x;
                    glm::vec3 v3 = m_verts[pv3].pos;
                    v3.y += 0.05f;

                    g_pApp->GetRenderer()->AddLine(v1, v3, glm::vec3(1, 1, 1));
                }
            }
        }
        */
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static float Barrycentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& pos) 
{
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);

    float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.z - p3.z)) / det;
    float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.z - p3.z)) / det;
    float l3 = 1.0f - l1 - l2;

    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

bool World::HeightAt(const glm::vec3& pos, float& _out_height)
{
    glm::ivec3 grid(0, 0, 0);
    grid.x = static_cast<int>(glm::floor(pos.x));
    grid.z = static_cast<int>(glm::floor(pos.z));

    if (!(0 <= grid.x && grid.x < m_extents.x - 1)
    || !(0 <= grid.z && grid.z < m_extents.z - 1))
    {
        return false;
    }

    glm::vec3 p, q, r;

    if ((pos.x - static_cast<float>(grid.x)) <= (static_cast<float>(grid.z + 1) - pos.z)) // Upper triangle
    {
        p = m_verts[(grid.x + 0) + m_extents.x * (grid.z + 0)].pos;
        q = m_verts[(grid.x + 1) + m_extents.x * (grid.z + 0)].pos;
        r = m_verts[(grid.x + 0) + m_extents.x * (grid.z + 1)].pos;

        _out_height = Barrycentric(p, q, r, pos);
    }
    else // Lower triangle
    {
        p = m_verts[(grid.x + 1) + m_extents.x * (grid.z + 0)].pos;
        q = m_verts[(grid.x + 0) + m_extents.x * (grid.z + 1)].pos;
        r = m_verts[(grid.x + 1) + m_extents.x * (grid.z + 1)].pos;

        _out_height = Barrycentric(p, q, r, pos);
    }

    return true;
}

bool World::Raycast(const Ray& ray, float& _out_t)
{
    float t = 0.01f;

    while (t < 800.0f)
    {
        glm::vec3 p = ray.origin + ray.direction * t;

        float height;
        if (HeightAt(p, height))
        {
            if (p.y < height)
            {
                _out_t = t - 0.005f;
                return true;
            } 
        }

        t += 0.01f;
    }

    return false;
}

std::vector<Vertex_UnlitTexturedColored*> World::GetVertsSelectedByBrush(const glm::vec3& pos, float radius)
{
    std::vector<Vertex_UnlitTexturedColored*> verts;

    for (Vertex_UnlitTexturedColored& vert : m_verts)
    {
        if (glm::length2(glm::vec2(vert.pos.x - pos.x, vert.pos.z - pos.z)) < (radius * radius))
        {
            verts.push_back(&vert);
        }
    }

    return verts;
}

void World::SaveHeightmap()
{
    stbi_flip_vertically_on_write(0);

    uint8_t* pHeightmapData = new uint8_t[32 * 32 * 4];

    for (int z = 0; z < m_extents.z; z++)
    {
        for (int x = 0; x < m_extents.x; x++)
        {
            uint32_t vertpos = x + m_extents.x * z;
            Vertex_UnlitTexturedColored& vert = m_verts[vertpos];

            uint32_t mgc = 16'777'216;

            float y = vert.pos.y;
            y += 32.0f;
            y /= 64.0f;

            uint32_t colorRGB = (uint32_t)(y *  16'777'215);

            uint8_t colorR = (colorRGB >> 16) & 0xff;
            uint8_t colorG = (colorRGB >> 8) & 0xff;
            uint8_t colorB = colorRGB & 0xff;

            long long heightMapDataOffset = (x + m_extents.x * z) * 4;
            pHeightmapData[heightMapDataOffset + 0] = colorR;
            pHeightmapData[heightMapDataOffset + 1] = colorR;
            pHeightmapData[heightMapDataOffset + 2] = colorR;
            pHeightmapData[heightMapDataOffset + 3] = 0xff;
        }
    }

    stbi_write_png("Assets/Textures/_Out_Heightmap32x32.png", 32, 32, 4, pHeightmapData, 32 * 4);

    delete[] pHeightmapData;
}

/*
    for (int z = 0; z < m_extents.z; z++)
    {
        for (int x = 0; x < m_extents.x; x++)
        {
            long long heightMapDataOffset = (x + z * m_extents.z) * numHeightmapChannels;
            uint8_t colorR = pHeightmapData[heightMapDataOffset + 0];
            uint8_t colorG = pHeightmapData[heightMapDataOffset + 1];
            uint8_t colorB = pHeightmapData[heightMapDataOffset + 2];

            uint32_t color = 0xffffffff;
            color =
                                ((colorR & 0xff) << 16) | 
                                ((colorG & 0xff) << 8 ) | 
                                ((colorB & 0xff));

            uint32_t mgc = 16'777'216;
            float y = color / (float)mgc;
            y *= 64.0f;
            y -= 32.0f;

            Vertex_UnlitColored vert;

            vert.pos = glm::vec3(0, 0, 0);
            // vert.pos.x = -m_extents.x / 2.0f + x;
            // vert.pos.z = -m_extents.z / 2.0f + z;
            vert.pos.x = x;
            vert.pos.y = y;
            vert.pos.z = z;

            vert.color = glm::vec3(1, 1, 1);

            m_verts.push_back(vert);
        }
    }
*/