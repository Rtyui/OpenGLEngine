#include "components/Camera.h"
#include "interface/Input.h"
#include "interface/TimeClock.h"
#include "interface/Debug.h"
#include "rendering/internal/UniformBlocksManager.h"
#include "components/Transform.h"

#include <vendor/glm/gtc/matrix_transform.hpp>
#include <vendor/glm/gtc/constants.hpp>
#include <vendor/glm/gtx/rotate_vector.hpp>
#include <memory>

Camera::Camera(const float fov, const float near, const float far) :
    m_Fov(fov), m_NearPlane(near), m_FarPlane(far), m_Forward({ 0.f, 0.f, -1.f }), m_Right({ 1.f, 0.f, 0.f }), m_ProjectionMatrix(1.f), m_ViewMatrix(1.f), m_UIProjectionMatrix(1.f)
{
    m_Aspect = (float)(Display::Singleton()->GetWidth()) / Display::Singleton()->GetHeight();
}

Camera::~Camera()
{
}

void Camera::Update()
{
    Move();
}

Component* Camera::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    Transform* transform = entity->GetComponent<Transform>();

    if (!transform)
    {
        Debug::Log(Debug::DebugLevel::Error, "Cannot create camera without transform!");
        return nullptr;
    }

    const char* fov_str = node.attribute("fov").value();
    float fov;
    if (sscanf_s(fov_str, "%f", &fov) != 1)
    {
        Debug::Log(Debug::Error, "Error while parsing camera fov data '" + std::string(fov_str) + "'!");
        return nullptr;
    }

    const char* near_str = node.attribute("near").value();
    float near;
    if (sscanf_s(near_str, "%f", &near) != 1)
    {
        Debug::Log(Debug::Error, "Error while parsing camera near data '" + std::string(near_str) + "'!");
        return nullptr;
    }

    const char* far_str = node.attribute("far").value();
    float far;
    if (sscanf_s(far_str, "%f", &far) != 1)
    {
        Debug::Log(Debug::Error, "Error while parsing camera far data '" + std::string(far_str) + "'!");
        return nullptr;
    }

    Camera* camera = new Camera(fov, near, far);

    camera->m_Entity = entity;
    camera->UpdateProjectionMatrix(true);
    camera->UpdateViewMatrix(true);
    camera->m_Entity = nullptr;

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
    Transform* transform = m_Entity->GetTransform();
    if (transform->HasRotationChanged())
    {
        //glm::vec2 rotation;
        //rotation.x = m_Transform->GetRotation().x - 360.f * (m_Transform->GetRotation().x > 360.f);
        //rotation.y = m_Transform->GetRotation().y - 360.f * (m_Transform->GetRotation().y > 360.f);

        m_Forward = glm::vec3(0.f, 0.f, -1.f);
        m_Forward = glm::rotate(m_Forward, transform->GetRotation().x, glm::vec3(1.f, 0.f, 0.f));
        m_Forward = glm::rotate(m_Forward, transform->GetRotation().y, glm::vec3(0.f, 1.f, 0.f));

        m_Right = glm::vec3(0.f, 0.f, -1.f);
        m_Right = glm::rotate(m_Right, transform->GetRotation().x, glm::vec3(1.f, 0.f, 0.f));
        m_Right = glm::rotate(m_Right, transform->GetRotation().y, glm::vec3(0.f, 1.f, 0.f));
    }

    m_ViewMatrix = glm::lookAt(transform->GetPosition(), transform->GetPosition() + m_Forward, glm::vec3(0.f, 1.f, 0.f));
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
        m_Entity->GetTransform()->IncreasePosition(glm::vec3(0.f, 0.f, -5.f) * TimeClock::Singleton()->GetDeltaTime());
    }
    else if (Input::Singleton()->GetKey(GLFW_KEY_S))
    {
        m_Entity->GetTransform()->IncreasePosition(glm::vec3(0.f, 0.f, 5.f) * TimeClock::Singleton()->GetDeltaTime());
    }
    if (Input::Singleton()->GetKey(GLFW_KEY_A))
    {
        m_Entity->GetTransform()->IncreasePosition(glm::vec3(-5.f, 0.f, 0.f) * TimeClock::Singleton()->GetDeltaTime());
    }
    else if (Input::Singleton()->GetKey(GLFW_KEY_D))
    {
        m_Entity->GetTransform()->IncreasePosition(glm::vec3(5.f, 0.f, 0.f) * TimeClock::Singleton()->GetDeltaTime());
    }
    if (Input::Singleton()->GetKey(GLFW_KEY_SPACE))
    {
        m_Entity->GetTransform()->IncreasePosition(glm::vec3(0.f, 5.f, 0.f) * TimeClock::Singleton()->GetDeltaTime());
    }
    else if (Input::Singleton()->GetKey(GLFW_KEY_LEFT_SHIFT))
    {
        m_Entity->GetTransform()->IncreasePosition(glm::vec3(0.f, -5.f, 0.f) * TimeClock::Singleton()->GetDeltaTime());
    }
    UpdateViewMatrix(true);
}
