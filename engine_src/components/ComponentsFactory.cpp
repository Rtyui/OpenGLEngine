#include "components/ComponentsFactory.h"
#include "components/Transform3D.h"
#include "components/TransformUI.h"
#include "components/MeshRender.h"
#include "components/SpriteRender.h"
#include "components/TextRender.h"
#include "interface/Debug.h"

std::unordered_map<std::string, Component::ComponentXMLCreator> ComponentsFactory::s_ComponentFactoryMap =
{
    {"TransformUI", TransformUI::CreateFromXMLNode},
    {"Transform3D", Transform3D::CreateFromXMLNode},
    {"MeshRender", MeshRender::CreateFromXMLNode},
    {"SpriteRender", SpriteRender::CreateFromXMLNode},
    {"TextRender", TextRender::CreateFromXMLNode}
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
