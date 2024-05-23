#pragma once

#include "Common.h"

#include <string>
#include <vector>

#include "Graphics3D/API/VertexBuffer.h"
#include "Graphics3D/API/VertexArray.h"
#include "Graphics3D/API/ProgramPipeline.h"
#include "Graphics3D/API/Texture.h"
#include "Graphics3D/Geometry.h"
#include "Graphics3D/Mesh.h"
#include "Graphics3D/SceneNodes.h"
#include "Graphics3D/Light.h"

////////////////////////////////////////////////////
//  class App
////////////////////////////////////////////////////

class App
{
 public:
    App();
    ~App();

    bool Init();
    void UpdateAndRender(const float fDeltaTime);

    void OnKeyDown(int key);
    void OnKeyUp(int key);
    void OnMouseMove(float x, float y);
    void OnMouseButtonDown(int button);
    void OnMouseButtonUp(int button);

 private:
    StrongMeshPtr m_Mesh_Rectangle;
    StrongMeshPtr m_Mesh_Cube;
    StrongMeshPtr m_Mesh_Terrain;

    StrongTexturePtr m_Texture_Sky;

    SceneNodeList m_SceneNodes;
    AlphaSceneList m_AlphaSceneNodes;
    LightSceneNodeList m_LightNodes;
    std::shared_ptr<CameraNode> m_Camera;
    std::shared_ptr<LightNode> m_DirectionalLightNode;
    std::shared_ptr<LightNode> m_PointLightNode;
    std::shared_ptr<LightNode> m_SpotLightNode;

    GLuint m_FramebufferID;
    GLuint m_TextureID_ColorBuffer;
    GLuint m_RenderBufferID;
    StrongTexturePtr m_Texture_ColorBuffer;

    bool m_bKeyStates[256];
    bool m_bCameraMoving;
    glm::vec2 m_PrevMousePos;
    glm::vec2 m_CurrentMousePos;

    float m_Yaw;
    float m_Pitch;
};