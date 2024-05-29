#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <memory>
#include <vector>

#include "SceneNodes.h"
#include "Lights.h"

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

    std::shared_ptr<CameraNode>& GetCamera() { return m_Camera; }

 private:
    std::vector<std::shared_ptr<SceneNode>> m_SceneNodes;
    std::vector<std::shared_ptr<LightNode>> m_LightNodes;
    std::vector<AlphaNode*> m_AlphaNodes;
    
    std::shared_ptr<CameraNode> m_Camera;
    std::shared_ptr<SkyNode> m_SkyNode;
};