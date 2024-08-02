#include "components/Transform.h"

#include <vendor/glm/gtc/matrix_transform.hpp>

Transform::Transform() :
    m_Position(0.f), m_Rotation(0.f), m_Scale(1.f), m_PositionChanged(false), m_RotationChanged(false), m_TransformationMatrix(1.f)
{
}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
    m_Position(position), m_Rotation(rotation), m_Scale(scale), m_PositionChanged(false), m_RotationChanged(false), m_TransformationMatrix(1.f)
{
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
