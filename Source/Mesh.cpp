#include "Mesh.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

//-----------------------------------------------------------------------------
// Mesh Implementation
//-----------------------------------------------------------------------------

Mesh::Mesh()
{
    m_numVerts = 0;
}

void Mesh::Load(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "r");
    
    std::vector<Vertex_LitTexturedColored> verts;

    std::vector<glm::vec3>  v;
    std::vector<glm::vec3>  vn;
    std::vector<glm::vec2>  vt;

    while (1)
    {
        char data[BUFSIZ];
        
        if (fscanf(fp, "%s", data) == EOF)
            break;

        if (strcmp(data, "v") == 0)
        {
            glm::vec3 pos;
            fscanf(fp, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
            v.push_back(pos);
        }
        else
        if (strcmp(data, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            vn.push_back(normal);
        }
        else
        if (strcmp(data, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(fp, "%f %f\n", &uv.x, &uv.y);
            vt.push_back(uv);
        }
        else
        if (strcmp(data, "f") == 0)
        {
            //  f = v -> vt -> vn

            struct ObjFace
            {
                uint32_t    vIndex;
                uint32_t    vtIndex;
                uint32_t    vnIndex;
            };

            const uint32_t faceCount = 3;
            ObjFace faces[faceCount];

            fscanf(
                fp, 
                "%u/%u/%u %u/%u/%u %u/%u/%u\n", 
                &faces[0].vIndex, &faces[0].vtIndex, &faces[0].vnIndex,
                &faces[1].vIndex, &faces[1].vtIndex, &faces[1].vnIndex, 
                &faces[2].vIndex, &faces[2].vtIndex, &faces[2].vnIndex);

            for (uint32_t i = 0; i < faceCount; i++)
            {
                ObjFace& face = faces[i];
                --face.vIndex;
                --face.vtIndex;
                --face.vnIndex;

                Vertex_LitTexturedColored vert;
                vert.pos = v[faces[i].vIndex]; vert.norm = vn[face.vnIndex]; vert.color = glm::vec3(1, 1, 1); vert.uv = vt[face.vtIndex];
                verts.push_back(vert);
            }
        }
    }

    fclose(fp);

    m_numVerts = verts.size();

    m_vertexArray.Init();
    m_vertexArray.SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_vertexArray.SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_vertexArray.SetVertexAttribute(0, 2, 3, GL_FLOAT, 24);
    m_vertexArray.SetVertexAttribute(0, 3, 2, GL_FLOAT, 36);

    long long vertBufferSize = sizeof(Vertex_LitTexturedColored) * verts.size();

    m_vertexBuffer.Init(vertBufferSize, GL_STATIC_DRAW);
    m_vertexBuffer.MapMemory(0, vertBufferSize, verts.data());

    m_vertexArray.SetVertexBuffer(0, &m_vertexBuffer, sizeof(Vertex_LitTexturedColored), VertexArrayInputRate_Vertex);
    
    verts.clear();
}

void Mesh::Render()
{
    m_vertexArray.Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_numVerts);
}