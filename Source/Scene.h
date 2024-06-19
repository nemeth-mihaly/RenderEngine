#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <memory>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

#include "SceneNodes.h"
#include "Lights.h"

#include "Mesh.h"
#include "Texture.h"

extern      StrongShaderPtr     g_TerrainShader;

constexpr int MAX_PARTICLES = 256;

class Particles
{
public:
    Particles() {}
    ~Particles() {}
};

////////////////////////////////////////////////////
//  class Scene
////////////////////////////////////////////////////

class Scene
{
public:
    Scene();
    ~Scene();

    void Update(const float deltaTime);
    void Render();

    std::shared_ptr<SceneNode> GetRoot() { return m_root; }

    void AddAlphaNode(AlphaNode* pAlphaNode) { m_AlphaNodes.push_back(pAlphaNode); }

    StrongMeshPtr GetMesh(const std::string& name);
    StrongTexturePtr GetTexture(const std::string& name);

    void ReloadTerrain();

    std::shared_ptr<CameraNode>& GetCamera() { return m_Camera; }

private:
    std::shared_ptr<SceneNode> m_root;

    std::unordered_map<std::string, StrongMeshPtr> m_Meshes;
    std::unordered_map<std::string, StrongTexturePtr> m_Textures;

    std::vector<std::shared_ptr<LightNode>> m_LightNodes;
    std::vector<AlphaNode*> m_AlphaNodes;
    
    std::shared_ptr<CameraNode> m_Camera;
    std::shared_ptr<CubeMapNode> m_CubeMap;
    std::shared_ptr<TerrainNode> m_Terrain;

    StrongUniformBufferPtr m_UniformBufferMatrices;
    
    LightProperties m_LightProperties[32];
    StrongUniformBufferPtr m_UniformBufferLighting;

    /** Particles */

    /*
    struct ParticleExtraVertexData
    {
        glm::vec3 Pos;
    };

    int                     m_ParticleVertexCount;
    StrongVertexBufferPtr   m_ParticleVertexBuffer;
    StrongVertexBufferPtr   m_ParticleExtraDataVertexBuffer;
    std::vector<ParticleExtraVertexData> m_ParticleStagingBuffer;
    StrongVertexArrayPtr    m_ParticleVertexArray;

    struct Particle
    {
        glm::vec3 Pos, Velocity;
        float Life {0.0f};
    };

    Particle m_Particles[MAX_PARTICLES];
    */
};