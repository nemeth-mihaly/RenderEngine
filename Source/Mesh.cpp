#include "Mesh.h"

////////////////////////////////////////////////////
//  Mesh_t Implementation
////////////////////////////////////////////////////

Mesh_t::Mesh_t()
{
}

Mesh_t::~Mesh_t()
{
}

void Mesh_t::LoadFromFile(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "r");
    assert(fp != NULL);

    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec2> Texcoords;

    std::vector<Vertex> Vertices;

    while (true)
    {
        char LineBuf[BUFSIZ];
        int ScanResult = fscanf(fp, "%s", LineBuf);
        
        if (ScanResult == EOF)
        {
            break;
        }

        if (strcmp(LineBuf, "v") == 0)
        {
            glm::vec3 Position;
            fscanf(fp, "%f %f %f\n", &Position.x, &Position.y, &Position.z);

            Positions.push_back(Position);
        }
        else
        if (strcmp(LineBuf, "vn") == 0)
        {
            glm::vec3 Normal;
            fscanf(fp, "%f %f %f\n", &Normal.x, &Normal.y, &Normal.z);

            Normals.push_back(Normal);
        }
        else
        if (strcmp(LineBuf, "vt") == 0)
        {
            glm::vec2 Texcoord;
            fscanf(fp, "%f %f\n", &Texcoord.x, &Texcoord.y);

            Texcoords.push_back(Texcoord);
        }
        else
        if (strcmp(LineBuf, "f") == 0)
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

            auto GetVertexFromFace = [&Positions, &Normals, &Texcoords](uint32_t Indices[3])
            {
                Vertex Vertex;

                Vertex.Pos = Positions[(Indices[0] - 1)];
                Vertex.Normal = Normals[(Indices[2] - 1)];
                Vertex.Uv = Texcoords[(Indices[1] - 1)];  

                return Vertex;
            };

            Vertices.push_back(GetVertexFromFace(FaceIndices1));
            Vertices.push_back(GetVertexFromFace(FaceIndices2));
            Vertices.push_back(GetVertexFromFace(FaceIndices3));
        }
    }

    fclose(fp);

    m_VertexArray.reset(new VertexArray());
    m_VertexArray->AddVertexBuffer(sizeof(Vertex), Vertices.size(), Vertices.data());
    m_VertexArray->SetAttribute(0, 3, GL_FLOAT, 0, 0);
    m_VertexArray->SetAttribute(1, 3, GL_FLOAT, 12, 0);
    m_VertexArray->SetAttribute(2, 2, GL_FLOAT, 24, 0);

    m_VertexCount = Vertices.size();
    Vertices.clear();
}