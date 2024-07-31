#include "components/TransformUI.h"
#include "interface/Debug.h"
#include "entities/Entity.h"

#include <vendor/glm/gtc/matrix_transform.hpp>

TransformUI::TransformUI() :
    m_Position(0.f), m_Rotation(0.f), m_Size(1.f)
{
    UpdateTransformationMatrix();
}

TransformUI::TransformUI(const glm::vec2& position, const float& rotation, const glm::vec2& size) :
    m_Position(position), m_Rotation(rotation), m_Size(size)
{
    UpdateTransformationMatrix();
}

void TransformUI::UpdateTransformationMatrix()
{
    m_TransformationMatrix = m_NoScaleTM = glm::mat4(1.f);
    m_NoScaleTM = glm::translate(m_NoScaleTM, glm::vec3(m_Position.x, -m_Position.y, 0.f));
    m_NoScaleTM = glm::rotate(m_NoScaleTM, m_Rotation, glm::vec3(0.f, 0.f, 1.f));
    m_TransformationMatrix = glm::scale(m_NoScaleTM, glm::vec3(m_Size, 1.f));
}

void TransformUI::IncreasePosition(glm::vec3 delta)
{
    Debug::Log(Debug::Warning, "P Using 3D movement for UI object '" + m_Entity->GetName() + "'!");
    
}

void TransformUI::IncreaseRotation(glm::vec3 delta)
{
    Debug::Log(Debug::Warning, "R Using 3D movement for UI object '" + m_Entity->GetName() + "'!");
}

void TransformUI::IncreasePosition(glm::vec2 delta)
{
    m_Position += delta;
    if (delta != glm::vec2(0.f))
    {
        UpdateTransformationMatrix();
    }
}

void TransformUI::IncreaseRotation(float delta)
{
    m_Rotation += delta;
    if (delta != 0.f)
    {
        UpdateTransformationMatrix();
    }
}


Component* TransformUI::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    glm::vec2 position(0.f);
    float rotation(0.f);
    glm::vec2 size(1.f);

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

Component* TransformUI::Create(const glm::vec2& position, const float& rotation, const glm::vec2& size)
{
    return new TransformUI(position, rotation, size);
}
