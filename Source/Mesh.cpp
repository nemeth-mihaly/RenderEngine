#include "Mesh.h"

////////////////////////////////////////////////////
//  Mesh Implementation
////////////////////////////////////////////////////

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::LoadResource(const std::string& name)
{
    FILE* fp = fopen(name.c_str(), "r");
    assert(fp != nullptr);

    std::vector<Vertex> vertices;

    std::vector<glm::vec3>      v;
    std::vector<glm::vec3>      vn;
    std::vector<glm::vec2>      vt;

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

                Vertex vertex;
                vertex.Pos = v[faces[i].vIndex]; vertex.Normal = vn[face.vnIndex]; vertex.Uv = vt[face.vtIndex];
                vertices.push_back(vertex);
            }
        }
    }

    fclose(fp);

    m_VertexArray.reset(new VertexArray());
    m_VertexArray->SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_VertexArray->SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_VertexArray->SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);

    m_VertexBuffer.reset(new VertexBuffer(sizeof(Vertex) * vertices.size(), GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, sizeof(Vertex) * vertices.size(), vertices.data());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    m_VertCount = vertices.size();
    vertices.clear();
}