#include "entities/Camera.h"
#include "interface/Input.h"
#include "interface/TimeClock.h"
#include "interface/Debug.h"
#include "rendering/internal/UniformBlocksManager.h"

#include <vendor/glm/gtc/matrix_transform.hpp>
#include <vendor/glm/gtc/constants.hpp>
#include <vendor/glm/gtx/rotate_vector.hpp>

Camera::Camera()
    : m_Position({ 0.f, 0.f, 0.f }), m_Rotation({ 0.f, 0.f, 0.f }), m_Forward({ 0.f, 0.f, -1.f }), m_Right({ 1.f, 0.f, 0.f }),
    m_LastPosition({ 0.f, 0.f, 0.f }), m_LastRotation({ 0.f, 0.f, 0.f }), m_UpdatedRotation(true), m_Fov(70.f), m_NearPlane(0.1f),
    m_FarPlane(100.f)
{
    m_Aspect = (float)(Display::Singleton()->GetWidth()) / Display::Singleton()->GetHeight();
    UpdateProjectionMatrix(false);
    UpdateViewMatrix(false);
}
Camera::Camera(glm::vec3 position)
    : m_Position(position), m_Rotation({ 0.f, 0.f, 0.f }), m_Forward({ 0.f, 0.f, -1.f }), m_Right({ 1.f, 0.f, 0.f }),
    m_LastPosition({ 0.f, 0.f, 0.f }), m_LastRotation({ 0.f, 0.f, 0.f }), m_UpdatedRotation(true), m_Fov(70.f), m_NearPlane(0.1f),
    m_FarPlane(100.f)
{
    m_Aspect = (float)(Display::Singleton()->GetWidth()) / Display::Singleton()->GetHeight();
    UpdateProjectionMatrix(false);
    UpdateViewMatrix(false);
}

Camera::~Camera()
{
}

void Camera::Update()
{
    Move();
}

Camera* Camera::CreateFromXMLNode(const pugi::xml_node& node)
{
    Camera* camera = new Camera();

    camera->m_Name = std::string(node.attribute("name").value());
    if (camera->m_Name == "")
    {
        Debug::Log(Debug::Error, "Missing camera name!");
        delete camera;
        return nullptr;
    }

    const char* positionData = node.attribute("position").value();
    if (sscanf_s(positionData, "%f %f %f", &camera->m_Position.x, &camera->m_Position.y, &camera->m_Position.z) != 3)
    {
        Debug::Log(Debug::Error, "Error while parsing camera position data '" + std::string(positionData) + "'!");
        delete camera;
        return nullptr;
    }

    const char* rotationData = node.attribute("rotation").value();
    if (sscanf_s(rotationData, "%f %f %f", &camera->m_Rotation.x, &camera->m_Rotation.y, &camera->m_Rotation.z) != 3)
    {
        Debug::Log(Debug::Error, "Error while parsing camera rotation data '" + std::string(rotationData) + "'!");
        delete camera;
        return nullptr;
    }

    const char* fov = node.attribute("fov").value();
    if (sscanf_s(fov, "%f", &camera->m_Fov) != 1)
    {
        Debug::Log(Debug::Error, "Error while parsing camera fov data '" + std::string(fov) + "'!");
        delete camera;
        return nullptr;
    }

    const char* near = node.attribute("near").value();
    if (sscanf_s(near, "%f", &camera->m_NearPlane) != 1)
    {
        Debug::Log(Debug::Error, "Error while parsing camera near data '" + std::string(near) + "'!");
        delete camera;
        return nullptr;
    }

    const char* far = node.attribute("far").value();
    if (sscanf_s(far, "%f", &camera->m_FarPlane) != 1)
    {
        Debug::Log(Debug::Error, "Error while parsing camera far data '" + std::string(far) + "'!");
        delete camera;
        return nullptr;
    }

    camera->UpdateProjectionMatrix(false);
    camera->UpdateViewMatrix(false);

    return camera;
}


void Camera::UpdateProjectionMatrix(bool updateUBO)
{
    m_ProjectionMatrix = glm::perspective(m_Fov, m_Aspect, m_NearPlane, m_FarPlane);
    m_UIProjectionMatrix = glm::ortho(0.f, (float)Display::Singleton()->GetWidth(), -(float)Display::Singleton()->GetHeight(), 0.f, -1.f, 1.f);
    if (!updateUBO)
    {
        return;
    }
    UniformBlocksManager::SetUniformBlockData((unsigned)UniformBlocksManager::UniformBlocksTypes::CameraMatrices,
        (unsigned)UniformBlocksManager::CameraMatrices::Projection, &m_ProjectionMatrix[0][0]);
    UniformBlocksManager::SetUniformBlockData((unsigned)UniformBlocksManager::UniformBlocksTypes::CameraMatrices,
        (unsigned)UniformBlocksManager::CameraMatrices::UIProjection, &m_UIProjectionMatrix[0][0]);
}

void Camera::UpdateViewMatrix(bool updateUBO)
{
    if (m_UpdatedRotation)
    {
        m_Rotation.x = m_Rotation.x - 360.f * (m_Rotation.x > 360.f);
        m_Rotation.y = m_Rotation.y - 360.f * (m_Rotation.y > 360.f);

        m_Forward = glm::vec3(0.f, 0.f, -1.f);
        m_Forward = glm::rotate(m_Forward, m_Rotation.x, glm::vec3(1.f, 0.f, 0.f));
        m_Forward = glm::rotate(m_Forward, m_Rotation.y, glm::vec3(0.f, 1.f, 0.f));

        m_Right = glm::vec3(0.f, 0.f, -1.f);
        m_Right = glm::rotate(m_Right, m_Rotation.x, glm::vec3(1.f, 0.f, 0.f));
        m_Right = glm::rotate(m_Right, m_Rotation.y, glm::vec3(0.f, 1.f, 0.f));
        m_UpdatedRotation = false;
    }

    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, glm::vec3(0.f, 1.f, 0.f));
    if (!updateUBO)
    {
        return;
    }
    UniformBlocksManager::SetUniformBlockData((unsigned)UniformBlocksManager::UniformBlocksTypes::CameraMatrices,
        (unsigned)UniformBlocksManager::CameraMatrices::View, &m_ViewMatrix[0][0]);
}

void Camera::Move()
{
    if (Input::Singleton()->GetKey(GLFW_KEY_W))
    {
        m_Position += glm::vec3(0.f, 0.f, -5.f) * TimeClock::Singleton()->GetDeltaTime();
    }
    else if (Input::Singleton()->GetKey(GLFW_KEY_S))
    {
        m_Position += glm::vec3(0.f, 0.f, 5.f) * TimeClock::Singleton()->GetDeltaTime();
    }
    if (Input::Singleton()->GetKey(GLFW_KEY_A))
    {
        m_Position += glm::vec3(-5.f, 0.f, 0.f) * TimeClock::Singleton()->GetDeltaTime();
    }
    else if (Input::Singleton()->GetKey(GLFW_KEY_D))
    {
        m_Position += glm::vec3(5.f, 0.f, 0.f) * TimeClock::Singleton()->GetDeltaTime();
    }
    if (Input::Singleton()->GetKey(GLFW_KEY_SPACE))
    {
        m_Position += glm::vec3(0.f, 5.f, 0.f) * TimeClock::Singleton()->GetDeltaTime();
    }
    else if (Input::Singleton()->GetKey(GLFW_KEY_LEFT_SHIFT))
    {
        m_Position += glm::vec3(0.f, -5.f, 0.f) * TimeClock::Singleton()->GetDeltaTime();
    }
    UpdateViewMatrix(true);
}
