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

void Mesh::LoadFromFile(const std::string& resource)
{
    FILE* fp = fopen(resource.c_str(), "r");
    assert(fp != NULL);

    std::vector<Vertex> vertices;

    std::vector<glm::vec3>      v;
    std::vector<glm::vec3>      vn;
    std::vector<glm::vec2>      vt;

    while (true)
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
            // [0]: v; [1]: vt; [2]: vn;
            uint32_t FaceIndices1[3];
            uint32_t FaceIndices2[3];
            uint32_t FaceIndices3[3];

            fscanf(
                fp, 
                "%u/%u/%u %u/%u/%u %u/%u/%u\n", 
                &FaceIndices1[0], &FaceIndices1[1], &FaceIndices1[2],
                &FaceIndices2[0], &FaceIndices2[1], &FaceIndices2[2], 
                &FaceIndices3[0], &FaceIndices3[1], &FaceIndices3[2]
            );

            auto GetVertexFromFace = [&v, &vn, &vt](uint32_t Indices[3])
            {
                Vertex vertex;

                vertex.Pos = v[(Indices[0] - 1)];
                vertex.Normal = vn[(Indices[2] - 1)];
                vertex.Uv = vt[(Indices[1] - 1)];  

                return vertex;
            };

            vertices.push_back(GetVertexFromFace(FaceIndices1));
            vertices.push_back(GetVertexFromFace(FaceIndices2));
            vertices.push_back(GetVertexFromFace(FaceIndices3));
        }
    }

    fclose(fp);

    m_VertexBuffer.reset(new VertexBuffer(sizeof(Vertex) * vertices.size(), GL_STATIC_DRAW));
    m_VertexBuffer->MapMemory(0, sizeof(Vertex) * vertices.size(), vertices.data());

    m_VertexArray.reset(new VertexArray());
    m_VertexArray->SetVertexBuffer(0, m_VertexBuffer, sizeof(Vertex), VertexArrayInputRate_Vertex);
    m_VertexArray->SetVertexAttribute(0, 0, 3, GL_FLOAT, 0);
    m_VertexArray->SetVertexAttribute(0, 1, 3, GL_FLOAT, 12);
    m_VertexArray->SetVertexAttribute(0, 2, 2, GL_FLOAT, 24);

    m_VertexCount = vertices.size();
    vertices.clear();
}