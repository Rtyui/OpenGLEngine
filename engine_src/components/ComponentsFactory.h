#pragma once

#include "components/Component.h"

#include <unordered_map>
#include <string>

class ComponentsFactory
{
public:
    static std::unordered_map<std::string, Component::ComponentXMLCreator> s_ComponentFactoryMap;
    static bool HasType(const std::string& name);
    static Component* CreateComponent(const std::string& name, const pugi::xml_node& node, Entity* entity);

};

