#pragma once

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <unordered_map>

#include "3rdParty/nlohmann/json.hpp"
using Json = nlohmann::json;

#include "SceneNodes.h"

//-----------------------------------------------------------------------------
// class SceneNodeFactory
//-----------------------------------------------------------------------------

class SceneNodeFactory
{
 public:
    void Init();

    std::shared_ptr<SceneNode> CreateSceneNode(const Json& data);
    
 private:
    std::shared_ptr<SceneNode> CreateSceneNodeBasedOnType(const std::string& type);

 private:
    std::unordered_map<std::string, uint8_t> m_typemap;
};