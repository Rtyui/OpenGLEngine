#pragma once
#include <vendor/glm/glm.hpp>
#include <memory>
#include <vendor/pugixml/pugixml.hpp>

class Light
{
private:
    std::string m_Name;
    glm::vec3 m_Position;
    glm::vec3 m_Color;
public:
    Light(glm::vec3 position, glm::vec3 color);

    inline const glm::vec3 GetPosition() const { return m_Position; }
    inline const glm::vec3 GetColor() const { return m_Color; }

    void Load();

    static Light* CreateFromXMLNode(const pugi::xml_node& node);

private:

    Light();
};
