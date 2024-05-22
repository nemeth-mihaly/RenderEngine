#include "AssetManager.h"

#include <cassert>
#include <iostream>

static char* _ReadFile(const std::string& name)
{
    FILE* fp = fopen(name.c_str(), "rb");
    assert(fp != nullptr);

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char* buf = new char[size + 1];
    size_t bytesRead = fread(buf, sizeof(char), size, fp);
    assert(bytesRead >= size);

    buf[size] = '\0';
    fclose(fp);

    return buf;
}

////////////////////////////////////////////////////
//  AssetManager Implementation
////////////////////////////////////////////////////

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::LoadShaderProgram(const std::string& shaderProgName, const std::string& vertShaderName, const std::string& fragShaderName)
{
    StrongProgramPipelinePtr shaderProgram;
    shaderProgram.reset(new ProgramPipeline());

    char* pVertShaderSource = _ReadFile(vertShaderName);
    char* pFragShaderSource = _ReadFile(fragShaderName);

    auto vertShader = std::make_shared<ShaderProgram>();
    assert(vertShader);
    vertShader->Create(GL_VERTEX_SHADER);
    vertShader->SetSource(1, &pVertShaderSource, nullptr);
    vertShader->Compile();

    auto fragShader = std::make_shared<ShaderProgram>();
    assert(fragShader);
    fragShader->Create(GL_FRAGMENT_SHADER);
    fragShader->SetSource(1, &pFragShaderSource, nullptr);
    fragShader->Compile();

    shaderProgram->Create();
    shaderProgram->AttachShaderProgramStage(vertShader);
    shaderProgram->AttachShaderProgramStage(fragShader);
    shaderProgram->Link();

    delete[] pFragShaderSource;
    delete[] pVertShaderSource;

    m_ShaderPrograms[shaderProgName] = shaderProgram;
}

StrongProgramPipelinePtr AssetManager::GetShaderProgram(const std::string& name)
{
    return m_ShaderPrograms.at(name);
}

void AssetManager::LoadWavefrontMesh(const std::string& name)
{
    FILE* fp = fopen(name.c_str(), "r");
    assert(fp != nullptr);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    std::vector<Vertex> vertices;

    while (true)
    {
        char line[512];
        int result = fscanf(fp, "%s", line);
        
        if (result == EOF)
        {
            break;
        }

        if (strcmp(line, "v") == 0)
        {
            glm::vec3 pos;
            fscanf(fp, "%f %f %f\n", &pos.x, &pos.y, &pos.z);
            positions.push_back(pos);
        }
        else
        if (strcmp(line, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        }
        else
        if (strcmp(line, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(fp, "%f %f\n", &uv.x, &uv.y);
            uvs.push_back(uv);
        }
        else
        if (strcmp(line, "f") == 0)
        {
            // [0]: v; [1]: vt; [2]: vn;
            uint32_t faceIndices1[3];
            uint32_t faceIndices2[3];
            uint32_t faceIndices3[3];

            fscanf(
                fp, 
                "%u/%u/%u %u/%u/%u %u/%u/%u\n", 
                &faceIndices1[0], &faceIndices1[1], &faceIndices1[2],
                &faceIndices2[0], &faceIndices2[1], &faceIndices2[2], 
                &faceIndices3[0], &faceIndices3[1], &faceIndices3[2]
            );

            auto GetVertexFromFace = [&positions, &normals, &uvs](uint32_t indices[3])
            {
                Vertex vertex;

                vertex.Pos = positions[(indices[0] - 1)];
                vertex.Normal = normals[(indices[2] - 1)];
                vertex.Uv = uvs[(indices[1] - 1)];  

                return vertex;
            };

            vertices.push_back(GetVertexFromFace(faceIndices1));
            vertices.push_back(GetVertexFromFace(faceIndices2));
            vertices.push_back(GetVertexFromFace(faceIndices3));
        }
    }

    fclose(fp);

    StrongMeshPtr mesh;
    mesh.reset(new Mesh());
    assert(mesh);
    mesh->VertexCount = vertices.size();
    mesh->pVertices = vertices.data();
    mesh->VertexBufferOffset = 0;
    mesh->Create();

    vertices.clear();

    m_Meshes[name] = mesh;
}

StrongMeshPtr AssetManager::GetMesh(const std::string& name)
{
    return m_Meshes.at(name);
}

void AssetManager::LoadTexture(const std::string& name)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, channelCount;
    uint8_t* pPixels = stbi_load(name.c_str(), &width, &height, &channelCount, 0);
    assert(pPixels);

    StrongTexturePtr texture;
    texture.reset(new Texture());

    texture->Create(GL_TEXTURE_2D);
    
    texture->SetParamateri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->SetParamateri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->SetParamateri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->SetParamateri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    texture->SetImage2D(0, TextureInternalFormat(channelCount), width, height, 0, TextureFormat(channelCount), pPixels);

    stbi_image_free(pPixels);
    m_Textures[name] = texture;
}

StrongTexturePtr AssetManager::GetTexture(const std::string& name)
{
    return m_Textures.at(name);
}