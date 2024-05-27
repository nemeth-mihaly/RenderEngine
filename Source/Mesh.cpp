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

void Mesh_t::LoadFromFile(const std::string& Filename)
{
    FILE* File = fopen(Filename.c_str(), "r");
    assert(File != nullptr);

    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec2> Texcoords;

    std::vector<Vertex_t> Vertices;

    while (true)
    {
        char LineBuf[BUFSIZ];
        int ScanResult = fscanf(File, "%s", LineBuf);
        
        if (ScanResult == EOF)
        {
            break;
        }

        if (strcmp(LineBuf, "v") == 0)
        {
            glm::vec3 Position;
            fscanf(File, "%f %f %f\n", &Position.x, &Position.y, &Position.z);

            Positions.push_back(Position);
        }
        else
        if (strcmp(LineBuf, "vn") == 0)
        {
            glm::vec3 Normal;
            fscanf(File, "%f %f %f\n", &Normal.x, &Normal.y, &Normal.z);

            Normals.push_back(Normal);
        }
        else
        if (strcmp(LineBuf, "vt") == 0)
        {
            glm::vec2 Texcoord;
            fscanf(File, "%f %f\n", &Texcoord.x, &Texcoord.y);

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
                File, 
                "%u/%u/%u %u/%u/%u %u/%u/%u\n", 
                &FaceIndices1[0], &FaceIndices1[1], &FaceIndices1[2],
                &FaceIndices2[0], &FaceIndices2[1], &FaceIndices2[2], 
                &FaceIndices3[0], &FaceIndices3[1], &FaceIndices3[2]
            );

            auto GetVertexFromFace = [&Positions, &Normals, &Texcoords](uint32_t Indices[3])
            {
                Vertex_t Vertex;

                Vertex.Position = Positions[(Indices[0] - 1)];
                Vertex.Normal = Normals[(Indices[2] - 1)];
                Vertex.Texcoord = Texcoords[(Indices[1] - 1)];  

                return Vertex;
            };

            Vertices.push_back(GetVertexFromFace(FaceIndices1));
            Vertices.push_back(GetVertexFromFace(FaceIndices2));
            Vertices.push_back(GetVertexFromFace(FaceIndices3));
        }
    }

    fclose(File);

    VertexArray.reset(new VertexArray_t(Vertices.data(), Vertices.size()));
    Vertices.clear();
}