#include "Camera.h"

#include <stdio.h>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include "3rdParty/GLFW/glfw3.h"

#include "Application.h"

//-----------------------------------------------------------------------------
// Camera Implementation
//-----------------------------------------------------------------------------

Camera::Camera()
{
    m_bActive = false;
    m_bDebugMode = false;

    m_view = glm::mat4();
    m_projection = glm::mat4();

    m_position = glm::vec3(0, 0, 0);
    m_targetPosition = glm::vec3(0, 0, 0);
}

Camera::~Camera()
{

}

void Camera::Init()
{
    m_position = glm::vec3(0.0f, 0.0f, 5.0f);
    m_targetPosition = glm::vec3(0.0f, 0.0f,-1.0f);

    m_view = glm::lookAt(m_position, (m_position + m_targetPosition), glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::ivec2 viewportSize = g_pApp->GetWindowSize();  // The viewport size always the same as the window size.

    m_projection = glm::perspective(glm::radians(45.0f), static_cast<float>(viewportSize.x) / viewportSize.y, 0.001f, 1000.0f);
}

void Camera::UpdateViewTransform()
{
    m_view = glm::lookAt(m_position, (m_position + m_targetPosition), glm::vec3(0.0f, 1.0f, 0.0f));
}

static glm::vec3 ScreenToWorld(const glm::vec2& v)
{
    glm::vec2 v_ndcs;
    v_ndcs.x = ((2.0f / g_pApp->GetWindowSize().x) * v.x) - 1.0f;
    v_ndcs.y = 1.0f - ((2.0f / g_pApp->GetWindowSize().y) * v.y);

    glm::vec4 v_clipSpace(v_ndcs.x, v_ndcs.y,-1.0f, 1.0f);
    
    glm::vec4 v_eyeSpace = glm::inverse(g_pApp->GetRenderer()->GetCamera()->GetProjection()) * v_clipSpace;
    v_eyeSpace = glm::vec4(v_eyeSpace.x, v_eyeSpace.y,-1.0f, 0.0f);

    glm::vec4 v_worSpace = glm::inverse(g_pApp->GetRenderer()->GetCamera()->GetView()) * v_eyeSpace;
    v_worSpace = glm::normalize(v_worSpace);

    return glm::vec3(v_worSpace);
}

static glm::vec3 rayDir(0, 0, 0);

void Camera::Render()
{
    if (m_bDebugMode)
    {
        if (m_bActive)
        {
            glm::vec2 p = g_pApp->GetMousePosition();
            rayDir = ScreenToWorld(p); 

            return;
        }

        g_pApp->GetRenderer()->AddCross(GetPosition(), 0.3f, glm::vec3(1, 1, 1));
        g_pApp->GetRenderer()->AddLine(GetPosition(), GetPosition() + (rayDir * 1000.0f), glm::vec3(1, 1, 1));
    }
}

void Camera::OnResize(int width, int height)
{
    if (width == 0 || height == 0)
    {
        return;
    }
    
    m_projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / height, 0.001f, 1000.0f);
}

//-----------------------------------------------------------------------------
// CameraController Implementation
//-----------------------------------------------------------------------------

CameraController::CameraController()
{
    m_camera = nullptr;

    memset(m_bKeys, static_cast<int>(false), sizeof(m_bKeys));

    m_bIsRightMouseButtonDown = false;

    m_yaw = -90.0f;
    m_pitch = 0.0f;
}

CameraController::~CameraController()
{

}

void CameraController::Init(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
}

void CameraController::Update(const float deltaTime)
{
    const glm::ivec2 currentMousePos = g_pApp->GetMousePosition();
    const glm::ivec2 deltaMousePos = currentMousePos - m_lastMousePos;
    m_lastMousePos = currentMousePos;

    if (!m_bIsRightMouseButtonDown)
    {
        g_pApp->ShowMouse();
        return;
    }

    g_pApp->HideMouse();

    const float cameraSpeed = 8.5f;

    const glm::vec3 upDirection = glm::vec3(0, 1, 0);
    const glm::vec3 rightDirection = glm::cross(m_camera->GetTargetPosition(), upDirection);

    if (m_bKeys[GLFW_KEY_W])
    {
        glm::vec3 position = m_camera->GetPosition();
        position += m_camera->GetTargetPosition() * cameraSpeed * deltaTime;

        m_camera->SetPosition(position); 
    }
    else if (m_bKeys[GLFW_KEY_S])
    {
        glm::vec3 position = m_camera->GetPosition();
        position -= m_camera->GetTargetPosition() * cameraSpeed * deltaTime;

        m_camera->SetPosition(position);
    }

    if (m_bKeys[GLFW_KEY_A])
    {
        glm::vec3 position = m_camera->GetPosition();
        position -= rightDirection * cameraSpeed * deltaTime;

        m_camera->SetPosition(position);
    }
    else if (m_bKeys[GLFW_KEY_D])
    {
        glm::vec3 position = m_camera->GetPosition();
        position += rightDirection * cameraSpeed * deltaTime;

        m_camera->SetPosition(position);   
    }

    if (m_bKeys[GLFW_KEY_LEFT_SHIFT])
    {
        glm::vec3 position = m_camera->GetPosition();
        position += upDirection * cameraSpeed * deltaTime;

        m_camera->SetPosition(position);
    }
    else if (m_bKeys[GLFW_KEY_LEFT_CONTROL])
    {
        glm::vec3 position = m_camera->GetPosition();
        position -= upDirection * cameraSpeed * deltaTime;

        m_camera->SetPosition(position);
    }

    if (!(deltaMousePos.x == 0 && deltaMousePos.y == 0))
    {
        const float rotationSpeed = 0.085f;
        m_yaw += static_cast<float>(deltaMousePos.x) * rotationSpeed;
        m_pitch += static_cast<float>(-deltaMousePos.y) * rotationSpeed;

        glm::vec3 targetPosition;
        targetPosition.x = cosf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));
        targetPosition.y = sinf(glm::radians(m_pitch));
        targetPosition.z = sinf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));
        
        m_camera->SetTargetPosition(glm::normalize(targetPosition));
    }

    m_camera->UpdateViewTransform();
}

void CameraController::ResetInputStates()
{
    memset(m_bKeys, static_cast<int>(false), sizeof(m_bKeys));

    m_bIsRightMouseButtonDown = false;
}

void CameraController::OnKeyDown(int key)
{
    if ((0 <= key && key <= MAX_KEYS))
    {
        m_bKeys[key] = true;
    }
}

void CameraController::OnKeyUp(int key)
{
    if ((0 <= key && key <= MAX_KEYS))
    {
        m_bKeys[key] = false;
    }
}

void CameraController::OnMouseMove(int x, int y)
{

}

void CameraController::OnMouseButtonDown(int button)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        m_bIsRightMouseButtonDown = true;
    }
}

void CameraController::OnMouseButtonUp(int button)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        m_bIsRightMouseButtonDown = false;
    }
}