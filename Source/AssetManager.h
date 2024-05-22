#pragma once

#include "Common.h"

#include <string>
#include <unordered_map>

#include "Graphics3D/API/ProgramPipeline.h"
#include "Graphics3D/Mesh.h"
#include "Graphics3D/API/Texture.h"

////////////////////////////////////////////////////
//  class AssetManager
////////////////////////////////////////////////////

class AssetManager
{
 public:
    AssetManager();
    ~AssetManager();

    void LoadShaderProgram(const std::string& shaderProgName, const std::string& vertShaderName, const std::string& fragShaderName);
    StrongProgramPipelinePtr GetShaderProgram(const std::string& name);

    void LoadWavefrontMesh(const std::string& name);
    StrongMeshPtr GetMesh(const std::string& name);

    void LoadTexture(const std::string& name);
    StrongTexturePtr GetTexture(const std::string& name);

 private:
    std::unordered_map<std::string, StrongProgramPipelinePtr> m_ShaderPrograms;
    std::unordered_map<std::string, StrongMeshPtr> m_Meshes;
    std::unordered_map<std::string, StrongTexturePtr> m_Textures;
};