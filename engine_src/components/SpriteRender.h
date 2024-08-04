#pragma once

#include "components/Component.h"
#include "rendering/internal/Texture.h"
#include "components/Renderable.h"

#include <vendor/glm/glm.hpp>

class Renderer;

class SpriteRender : public Renderable
{
private:

    Texture* m_Sprite;
    glm::vec4 m_Color;


public:
    SpriteRender();
    SpriteRender(Texture* texture, const glm::vec4& color);

    virtual void Update() {};

    inline const glm::vec4 GetColor() const { return m_Color; }
    inline Texture* GetSprite() const { return m_Sprite; }

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);
    static Component* Create(Texture* texture, const glm::vec4& color);

};

