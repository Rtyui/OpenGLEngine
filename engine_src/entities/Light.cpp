#include "Light.h"
#include "rendering/internal/UniformBlocksManager.h"
#include "interface/Debug.h"

Light::Light(glm::vec3 position, glm::vec3 color) :
    m_Name("default"), m_Position(position), m_Color(color)
{
}

Light::Light() :
    m_Name("default"), m_Position(0.f, 0.f, 0.f), m_Color(1.f, 1.f, 1.f)
{
}

void Light::Load()
{
    UniformBlocksManager::SetUniformBlockData((unsigned)UniformBlocksManager::UniformBlocksTypes::LightData,
        (unsigned)UniformBlocksManager::LightData::Position, &m_Position[0]);

    UniformBlocksManager::SetUniformBlockData((unsigned)UniformBlocksManager::UniformBlocksTypes::LightData,
        (unsigned)UniformBlocksManager::LightData::Color, &m_Color[0]);
}

Light* Light::CreateFromXMLNode(const pugi::xml_node& node)
{
    Light* light = new Light();

    light->m_Name = std::string(node.attribute("name").value());
    if (light->m_Name == "")
    {
        Debug::Log(Debug::Error, "Missing light name!");
        delete light;
        return nullptr;
    }

    const char* positionData = node.attribute("position").value();
    if (sscanf_s(positionData, "%f %f %f", &light->m_Position.x, &light->m_Position.y, &light->m_Position.z) != 3)
    {
        Debug::Log(Debug::Error, "Error while parsing light position data '" + std::string(positionData) + "'!");
        delete light;
        return nullptr;
    }

    const char* colorData = node.attribute("color").value();
    if (sscanf_s(colorData, "%f %f %f", &light->m_Color.x, &light->m_Color.y, &light->m_Color.z) != 3)
    {
        Debug::Log(Debug::Error, "Error while parsing light color data '" + std::string(colorData) + "'!");
        delete light;
        return nullptr;
    }

    return light;
}
