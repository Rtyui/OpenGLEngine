#include "ui/components/SpriteRender.h"
#include "Resources.h"
#include "interface/Debug.h"
#include "rendering/Renderer.h"
#include "utils/XMLUtils.h"

SpriteRender::SpriteRender() :
    m_Sprite(Resources::Singleton()->GetTexture("white")), m_Color(1.f)
{
}

SpriteRender::SpriteRender(Texture* texture, const glm::vec4& color) :
    m_Sprite(texture), m_Color(color)
{
}

Component* SpriteRender::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    Texture* texture = nullptr;
    glm::vec4 color(1.f);

    if (!ParseTextureFromXMLNode(node, &texture) ||
        !ParseVec4FromXMLNode(node, "color", color, false))
    {
        return nullptr;
    }

    return new SpriteRender(texture, color);
}

Component* SpriteRender::Create(Texture* texture, const glm::vec4& color)
{
    return new SpriteRender(texture, color);
}
