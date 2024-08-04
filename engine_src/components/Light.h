#pragma once
#include "components/Component.h"

#include <vendor/glm/glm.hpp>
#include <memory>
#include <vendor/pugixml/pugixml.hpp>

class Light : public Component
{
private:
    glm::vec3 m_Color;
public:
    Light(const glm::vec3& color);

    inline const glm::vec3 GetColor() const { return m_Color; }

    void Load();

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);

private:

    Light();

    // Inherited via Component
    void Update() override;
};
