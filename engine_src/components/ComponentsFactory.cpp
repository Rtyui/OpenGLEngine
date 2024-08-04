#include "components/ComponentsFactory.h"
#include "components/Transform.h"
#include "components/MeshRender.h"
#include "components/SpriteRender.h"
#include "components/TextRender.h"
#include "components/Camera.h"
#include "components/Light.h"
#include "interface/Debug.h"

std::unordered_map<std::string, Component::ComponentXMLCreator> ComponentsFactory::s_ComponentFactoryMap =
{
    {"Transform", Transform::CreateFromXMLNode},
    {"MeshRender", MeshRender::CreateFromXMLNode},
    {"SpriteRender", SpriteRender::CreateFromXMLNode},
    {"TextRender", TextRender::CreateFromXMLNode},
    {"Camera", Camera::CreateFromXMLNode},
    {"Light", Light::CreateFromXMLNode}
};

bool ComponentsFactory::HasType(const std::string& name)
{
    return s_ComponentFactoryMap.find(name) != s_ComponentFactoryMap.end();
}

Component* ComponentsFactory::CreateComponent(const std::string& name, const pugi::xml_node& node, Entity* entity)
{
    const auto& it = s_ComponentFactoryMap.find(name);
    if (it == s_ComponentFactoryMap.end())
    {
        Debug::Log(Debug::Error, "Factory does not have constructor for component '" + name + "'!");
        return nullptr;
    }
    return it->second(node, entity);
}
