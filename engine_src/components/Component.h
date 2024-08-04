#pragma once

#include "entities/Entity.h"

#include <memory>
#include <vendor/pugixml/pugixml.hpp>

class Component
{
public:
    typedef Component* (*ComponentXMLCreator)(const pugi::xml_node& node, Entity* entity);
protected:

    Entity* m_Entity;

    bool m_Active;

public:
    Component();
    virtual ~Component() = default;
    Component(Entity* entity);

    virtual void Load() {};
    virtual void Unload() {};
    virtual void UpdateP();
    virtual void Update() = 0;

    inline void SetEntity(Entity* entity) { m_Entity = entity; }
    inline Entity* GetEntity() { return m_Entity; }
    inline const bool IsActive() const { return m_Active && m_Entity->IsActive(); }

    virtual void SetActive(const bool& active);

};
