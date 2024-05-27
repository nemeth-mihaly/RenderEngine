#include "Resources/ResourceManager.h"

#include <cassert>
#include <iostream>
#include <vector>

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
//  ResourceManager Implementation
////////////////////////////////////////////////////

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::LoadProgramPipeline(const std::string& shaderProgName)
{
    std::vector<StrongShaderProgramPtr> shaderPrograms;

    FILE* fp = fopen(shaderProgName.c_str(), "r");
    assert(fp != nullptr);

    while (true)
    {
        char line[512];
        int result = fscanf(fp, "%s", line);
        
        if (result == EOF)
            break;

        if (strcmp(line, "vs") == 0)
        {
            char buf[128];
            fscanf(fp, "%s\n", buf);

            char* pSource = _ReadFile(std::string(buf));

            StrongShaderProgramPtr vs(new ShaderProgram());
            vs->Create(GL_VERTEX_SHADER);
            vs->SetSource(1, &pSource, nullptr);
            vs->Compile();

            shaderPrograms.push_back(vs);

            delete[] pSource;
        }
        else
        if (strcmp(line, "fs") == 0)
        {
            char buf[128];
            fscanf(fp, "%s\n", buf);

            char* pSource = _ReadFile(std::string(buf));

            StrongShaderProgramPtr fs(new ShaderProgram());
            fs->Create(GL_FRAGMENT_SHADER);
            fs->SetSource(1, &pSource, nullptr);
            fs->Compile();

            shaderPrograms.push_back(fs);

            delete[] pSource;
        }
    }

    fclose(fp);

    StrongProgramPipelinePtr programPipeline;
    programPipeline.reset(new ProgramPipeline());

    programPipeline->Create();

    for (auto& shaderProgram : shaderPrograms)
        programPipeline->AttachShaderProgramStage(shaderProgram);

    programPipeline->Link();

    m_ShaderPrograms[shaderProgName] = programPipeline;
}

StrongProgramPipelinePtr ResourceManager::GetProgramPipeline(const std::string& name)
{
    return m_ShaderPrograms.at(name);
}