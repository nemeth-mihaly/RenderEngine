#include "SceneNodeFactory.h"

#include <stdio.h>
#include <iostream>

//-----------------------------------------------------------------------------
// enum SceneNodeType
//-----------------------------------------------------------------------------

enum SceneNodeType
{
    SceneNodeType_PawnNode,
    SceneNodeType_TestNode,
    SceneNodeType_MeshNode,
    SceneNodeType_LightNode
};

//-----------------------------------------------------------------------------
// SceneNodeFactory Implementation
//-----------------------------------------------------------------------------

void SceneNodeFactory::Init()
{
    m_typemap.insert(std::make_pair(std::string(PawnNode::g_type), SceneNodeType_PawnNode));
    m_typemap.insert(std::make_pair(std::string(TestNode::g_type), SceneNodeType_TestNode));
    m_typemap.insert(std::make_pair(std::string(MeshNode::g_type), SceneNodeType_MeshNode));
    m_typemap.insert(std::make_pair(std::string(LightNode::g_type), SceneNodeType_LightNode));
}

std::shared_ptr<SceneNode> SceneNodeFactory::CreateSceneNode(const Json& data)
{
    std::shared_ptr<SceneNode> node;
    
    if (data.contains("type"))
    {
        const std::string type = data["type"];

        node = CreateSceneNodeBasedOnType(type);
        node->Init(data);

        for (Json::const_iterator it = data["children"].begin(); it != data["children"].end(); it++)
        {
            node->AddChild(CreateSceneNode((*it)));
        }
    }

    node->PostInit();

    return node;
}

std::shared_ptr<SceneNode> SceneNodeFactory::CreateSceneNodeBasedOnType(const std::string& type)
{
    uint8_t hashedType = m_typemap.at(type.c_str());

    switch (hashedType)
    {
        case SceneNodeType_PawnNode:
            return std::make_shared<PawnNode>();

        case SceneNodeType_TestNode:
            return std::make_shared<TestNode>();

        case SceneNodeType_MeshNode:
            return std::make_shared<MeshNode>();

        case SceneNodeType_LightNode:
            return std::make_shared<LightNode>();
    }

    assert(1 && "Unknown SceneNode type!");
    return std::make_shared<PawnNode>();
} 