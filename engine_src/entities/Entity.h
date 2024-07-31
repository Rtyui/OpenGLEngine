#pragma once

#include <vendor/glm/glm.hpp>
#include <vector>
#include <vendor/pugixml/pugixml.hpp>

class Component;
class Transform;

class Entity
{
private:
    std::string m_Name;

    std::vector<Component*> m_Components;
    Transform* m_Transform;

    bool m_Active;

public:
    ~Entity();

    void Update();

    void AddComponent(Component* component);

    template <class T>
    T* GetComponent()
    {
        for (Component* component : m_Components)
        {
            T* ptr = dynamic_cast<T*>(component);
            if (ptr != nullptr)
            {
                return ptr;
            }
        }
        return nullptr;
    }

    template <class T>
    void GetComponents(std::vector<T*>& componentsContainer)
    {
        for (Component* component : m_Components)
        {
            T* ptr = dynamic_cast<T*>(component);
            if (ptr != nullptr)
            {
                componentsContainer.push_back(ptr);
            }
        }
    }

    const inline std::string& GetName() const { return m_Name; }
    inline Transform* GetTransform() const { return m_Transform; }
    inline const bool& IsActive() const { return m_Active; }

    void SetActive(const bool& active);

    static Entity* CreateFromXMLNode(const pugi::xml_node& node);
    static Entity* Create(const std::string& name);

private:

    Entity(const std::string& name);

};
