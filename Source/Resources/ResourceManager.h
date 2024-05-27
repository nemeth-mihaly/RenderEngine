#pragma once

#include "Common.h"

#include <string>
#include <unordered_map>

#include "Graphics3D/API/ProgramPipeline.h"

////////////////////////////////////////////////////
//  class ResourceManager
////////////////////////////////////////////////////

class ResourceManager
{
 public:
    ResourceManager();
    ~ResourceManager();

    void LoadProgramPipeline(const std::string& shaderProgName);
    StrongProgramPipelinePtr GetProgramPipeline(const std::string& name);

 private:
    std::unordered_map<std::string, StrongProgramPipelinePtr> m_ShaderPrograms;
};