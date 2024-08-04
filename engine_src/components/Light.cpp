#include "components/Light.h"
#include "components/Transform.h"
#include "rendering/internal/UniformBlocksManager.h"
#include "interface/Debug.h"
#include "utils/XMLUtils.h"

Light::Light(const glm::vec3& color) :
    m_Color(color)
{
}

Light::Light() :
   m_Color(1.f, 1.f, 1.f)
{
}

void Light::Update()
{
}

void Light::Load()
{
    UniformBlocksManager::SetUniformBlockData((unsigned)UniformBlocksManager::UniformBlocksTypes::LightData,
        (unsigned)UniformBlocksManager::LightData::Position, &m_Entity->GetTransform()->GetPosition()[0]);

    UniformBlocksManager::SetUniformBlockData((unsigned)UniformBlocksManager::UniformBlocksTypes::LightData,
        (unsigned)UniformBlocksManager::LightData::Color, &m_Color[0]);
}

Component* Light::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    glm::vec3 color = glm::vec3(1.f, 0.2f, 1.f);

    if (!ParseVec3FromXMLNode(node, "color", color, false))
    {
        return nullptr;
    }

    return new Light(color);
}
