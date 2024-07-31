#include "entities/Entity.h"
#include "interface/Debug.h"
#include "components/Component.h"
#include "components/Transform.h"

#include <vendor/glm/gtc/matrix_transform.hpp>
#include <sstream>

Entity::Entity(const std::string& name) :
    m_Transform(nullptr), m_Name(name), m_Active(true)
{
}

Entity::~Entity()
{
    for (auto* component : m_Components)
    {
        delete component;
    }
}

void Entity::Update()
{
    for (Component* component : m_Components)
    {
        component->Update();
    }
}

void Entity::AddComponent(Component* component)
{
    if (!component)
    {
        Debug::Log(Debug::Warning, "Component null!");
        return;
    }
    component->SetEntity(this);
    m_Components.push_back(component);

    if (m_Transform)
    {
        return;
    }

    Transform* transform = dynamic_cast<Transform*>(component);
    if (transform)
    {
        m_Transform = transform;
    }
}

void Entity::SetActive(const bool& active)
{
    m_Active = active;
}

Entity* Entity::CreateFromXMLNode(const pugi::xml_node& node)
{
    std::string name("");

    name = std::string(node.attribute("name").value());
    if (name == "")
    {
        Debug::Log(Debug::Error, "Missing entity name!");
        return nullptr;
    }
    
    return new Entity(name);
}

Entity* Entity::Create(const std::string& name)
{
    return new Entity(name);
}
