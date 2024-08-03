#include "components/Transform.h"
#include "interface/Debug.h"
#include "Utils.h"

#include <vendor/glm/gtc/matrix_transform.hpp>

Transform::Transform() :
    m_Position(0.f), m_Rotation(0.f), m_Scale(1.f), m_PositionChanged(false), m_RotationChanged(false), m_TransformationMatrix(1.f), m_NoScaleTM(1.f), m_UI(false)
{
    UpdateTransformationMatrix();
}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const bool& ui) :
    m_Position(position), m_Rotation(rotation), m_Scale(scale), m_PositionChanged(false), m_RotationChanged(false), m_TransformationMatrix(1.f), m_NoScaleTM(1.f), m_UI(ui)
{
    UpdateTransformationMatrix();
}

void Transform::Update()
{
    m_PositionChanged = (m_Position != m_LastPosition);
    m_RotationChanged = (m_Rotation != m_LastRotation);

    m_LastPosition = m_Position;
    m_LastRotation = m_Rotation;
}

void Transform::IncreasePosition(glm::vec3 delta)
{
    m_Position += delta;
    if (delta != glm::vec3(0.f))
    {
        UpdateTransformationMatrix();
    }
}

void Transform::IncreaseRotation(glm::vec3 delta)
{
    m_Rotation += delta;
    if (delta != glm::vec3(0.f))
    {
        UpdateTransformationMatrix();
    }
}

void Transform::UpdateTransformationMatrix()
{
    if (m_UI)
    {
        m_TransformationMatrix = m_NoScaleTM = glm::mat4(1.f);
        m_NoScaleTM = glm::translate(m_NoScaleTM, glm::vec3(m_Position.x, -m_Position.y, 0.f));
        m_NoScaleTM = glm::rotate(m_NoScaleTM, m_Rotation.z, glm::vec3(0.f, 0.f, 1.f));
        m_TransformationMatrix = glm::scale(m_NoScaleTM, m_Scale);
        return;
    }

    m_TransformationMatrix = glm::mat4(1.f);
    m_TransformationMatrix = glm::translate(m_TransformationMatrix, m_Position);
    m_TransformationMatrix = glm::rotate(m_TransformationMatrix, m_Rotation.x, glm::vec3(1.f, 0.f, 0.f));
    m_TransformationMatrix = glm::rotate(m_TransformationMatrix, glm::radians(m_Rotation.y), glm::vec3(0.f, 1.f, 0.f));
    m_TransformationMatrix = glm::rotate(m_TransformationMatrix, m_Rotation.z, glm::vec3(0.f, 0.f, 1.f));
    m_TransformationMatrix = glm::scale(m_TransformationMatrix, m_Scale);
}

bool Transform::IsScreenPointOn(glm::vec2 position) const
{
    if (m_UI)
    {
        return (position.x >= m_Position.x) && (position.x <= (m_Position.x + m_Scale.x)) &&
            (position.y >= m_Position.y) && (position.y <= (m_Position.y + m_Scale.y));
    }
    return false;
}


Component* Transform::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    glm::vec3 position(0.f);
    glm::vec3 rotation(0.f);
    glm::vec3 scale(1.f);
    bool ui = false;

    if (!Utils::ParseVec3FromXMLNode(node, "position", position, false) ||
        !Utils::ParseVec3FromXMLNode(node, "rotation", rotation, false) ||
        !Utils::ParseVec3FromXMLNode(node, "scale", scale, false) ||
        !Utils::ParseBoolFromXMLNode(node, "ui", ui, true))
    {
        return nullptr;
    }

    return new Transform(position, rotation, scale, ui);
}

Component* Transform::Create(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& size, const bool& ui)
{
    return new Transform(position, rotation, size, ui);
}
