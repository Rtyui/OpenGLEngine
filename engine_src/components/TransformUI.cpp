#include "components/TransformUI.h"
#include "interface/Debug.h"
#include "entities/Entity.h"
#include "interface/Display.h"

#include <vendor/glm/gtc/matrix_transform.hpp>

TransformUI::TransformUI() :
    Transform()
{
    UpdateTransformationMatrix();
}

TransformUI::TransformUI(const glm::vec3& position, const float& rotation, const glm::vec3& size) :
    Transform(position, glm::vec3(0.f, 0.f, rotation), size)
{
    UpdateTransformationMatrix();
}

void TransformUI::UpdateTransformationMatrix()
{
    m_TransformationMatrix = m_NoScaleTM = glm::mat4(1.f);
    m_NoScaleTM = glm::translate(m_NoScaleTM, glm::vec3(m_Position.x, -m_Position.y, 0.f));
    m_NoScaleTM = glm::rotate(m_NoScaleTM, m_Rotation.z, glm::vec3(0.f, 0.f, 1.f));
    m_TransformationMatrix = glm::scale(m_NoScaleTM, m_Scale);
}

bool TransformUI::IsScreenPointOn(glm::vec2 position)
{
    return (position.x >= m_Position.x) && (position.x <= (m_Position.x + m_Scale.x)) &&
        (position.y >= m_Position.y) && (position.y <= (m_Position.y + m_Scale.y));
}


Component* TransformUI::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    glm::vec3 position(0.f);
    float rotation(0.f);
    glm::vec3 size(1.f);

    const char* positionData = node.attribute("position").value();
    if (positionData[0] && (sscanf_s(positionData, "%f %f", &position.x, &position.y) != 2))
    {
        Debug::Log(Debug::Error, "Error while parsing ui position data '" + std::string(positionData) + "'!");
        return nullptr;
    }

    const char* rotationData = node.attribute("rotation").value();
    if (rotationData[0])
    {
        rotation = (float)atof(rotationData);
    }

    const char* sizeData = node.attribute("size").value();
    if (sizeData[0] && (sscanf_s(sizeData, "%f %f", &size.x, &size.y) != 2))
    {
        Debug::Log(Debug::Error, "Error while parsing ui size data '" + std::string(sizeData) + "'!");
        return nullptr;
    }

    return new TransformUI(position, rotation, size);
}

Component* TransformUI::Create(const glm::vec3& position, const float& rotation, const glm::vec3& size)
{
    return new TransformUI(position, rotation, size);
}
