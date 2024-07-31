#include "components/SpriteRender.h"
#include "Resources.h"
#include "interface/Debug.h"
#include "rendering/Renderer.h"

SpriteRender::SpriteRender() :
    m_Sprite(Resources::Singleton()->GetTexture("white")), m_Color(1.f)
{
}

SpriteRender::SpriteRender(Texture* texture, const glm::vec4& color) :
    m_Sprite(texture), m_Color(color)
{
}

void SpriteRender::GetSubmitted()
{
    Renderer::Singleton()->Submit(this);
}

void SpriteRender::GetUnSubmitted()
{
    Renderer::Singleton()->UnSubmit(this);
}

Component* SpriteRender::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    const std::string& textureName = node.attribute("sprite").value();
    Texture* texture = Resources::Singleton()->GetTexture(textureName);

    glm::vec4 color(1.f);
    const char* colorData = node.attribute("color").value();
    if (colorData[0] && (sscanf_s(colorData, "%f %f %f %f", &color.r, &color.g, &color.b, &color.a) != 4))
    {
        Debug::Log(Debug::Error, "Error while parsing sprite color data '" + std::string(colorData) + "'!");
        return nullptr;
    }


    return new SpriteRender(texture, color);
}

Component* SpriteRender::Create(Texture* texture, const glm::vec4& color)
{
    return new SpriteRender(texture, color);
}
