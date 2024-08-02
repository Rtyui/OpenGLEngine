#include "Transform3D.h"
#include "interface/Debug.h"
#include "entities/Entity.h"

#include <vendor/glm/gtc/matrix_transform.hpp>

Transform3D::Transform3D() :
    Transform()
{
    UpdateTransformationMatrix();
}

Transform3D::Transform3D(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
    Transform(position, rotation, scale)
{
    UpdateTransformationMatrix();
}

void Transform3D::UpdateTransformationMatrix()
{
    m_TransformationMatrix = glm::mat4(1.f);
    m_TransformationMatrix = glm::translate(m_TransformationMatrix, m_Position);
    m_TransformationMatrix = glm::rotate(m_TransformationMatrix, m_Rotation.x, glm::vec3(1.f, 0.f, 0.f));
    m_TransformationMatrix = glm::rotate(m_TransformationMatrix, glm::radians(m_Rotation.y), glm::vec3(0.f, 1.f, 0.f));
    m_TransformationMatrix = glm::rotate(m_TransformationMatrix, m_Rotation.z, glm::vec3(0.f, 0.f, 1.f));
    m_TransformationMatrix = glm::scale(m_TransformationMatrix, m_Scale);
}

void Transform3D::Update()
{
    m_PositionChanged = (m_Position != m_LastPosition);
    m_RotationChanged = (m_Rotation != m_LastRotation);

    m_LastPosition = m_Position;
    m_LastRotation = m_Rotation;
}

Component* Transform3D::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    glm::vec3 position(0.f);
    glm::vec3 rotation(0.f);
    glm::vec3 scale(1.f);

    const char* positionData = node.attribute("position").value();
    if (positionData[0] && (sscanf_s(positionData, "%f %f %f", &position.x, &position.y, &position.z) != 3))
    {
        Debug::Log(Debug::Error, "Error while parsing 3d position data '" + std::string(positionData) + "'!");
        return nullptr;
    }

    const char* rotationData = node.attribute("rotation").value();
    if (rotationData[0] && (sscanf_s(rotationData, "%f %f %f", &rotation.x, &rotation.y, &rotation.z) != 3))
    {
        Debug::Log(Debug::Error, "Error while parsing 3d rotation data '" + std::string(rotationData) + "'!");
        return nullptr;
    }

    const char* scaleData = node.attribute("scale").value();
    if (scaleData[0] && (sscanf_s(scaleData, "%f %f %f", &scale.x, &scale.y, &scale.z) != 3))
    {
        Debug::Log(Debug::Error, "Error while parsing 3d scale data '" + std::string(scaleData) + "'!");
        return nullptr;
    }

    return new Transform3D(position, rotation, scale);
}
