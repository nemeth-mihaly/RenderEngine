#pragma once

#include <stddef.h>
#include <stdint.h>
#include <memory>

#include "3rdParty/glm/glm.hpp"
#include "3rdParty/glm/gtc/matrix_transform.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"
#include "3rdParty/glm/gtc/quaternion.hpp"
#define GLM_ENABLE_EXPERIMENTAL 
#include "3rdParty/glm/gtx/quaternion.hpp"

//-----------------------------------------------------------------------------
// class Camera
//-----------------------------------------------------------------------------

class Camera
{
 public:
    bool        m_bActive;
    bool        m_bDebugMode; // Enables rendering of the frustum and the position as a cross.

    Camera();
    ~Camera();

    void Init();
    void UpdateViewTransform(); // Should be called before the rendering process.
    void Render();

    glm::vec3 ScreenPointToWorldDirection(const glm::vec2& point);

    void SetPosition(const glm::vec3& v) { m_position = v; }
    const glm::vec3& GetPosition() const { return m_position; }

    void SetTargetPosition(const glm::vec3& v) { m_targetPosition = v; }
    const glm::vec3& GetTargetPosition() const { return m_targetPosition; }

    const glm::mat4& GetView() const { return m_view; }
    const glm::mat4& GetProjection() const { return m_projection; }
 
    void OnResize(int width, int height);

 private:
    glm::vec3   m_position;
    glm::vec3   m_targetPosition;

    glm::mat4   m_view;
    glm::mat4   m_projection;
};

//-----------------------------------------------------------------------------
// class CameraController
//-----------------------------------------------------------------------------

class CameraController
{
 public:
    CameraController();
    ~CameraController();

    void Init(std::shared_ptr<Camera> camera);
    void Update(const float deltaTime);

    void ResetInputStates();

    void OnKeyDown(int key);
    void OnKeyUp(int key);

    void OnMouseMove(int x, int y);
    void OnMouseButtonDown(int button);
    void OnMouseButtonUp(int button);

 private:
    std::shared_ptr<Camera>     m_camera;

    static const int MAX_KEYS = 512;
    bool            m_bKeys[MAX_KEYS];

    glm::ivec2      m_lastMousePos;

    bool            m_bIsRightMouseButtonDown;

    float           m_yaw;
    float           m_pitch;
};