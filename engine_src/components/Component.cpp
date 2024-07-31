#include "components/Component.h"


Component::Component() :
    m_Entity(nullptr), m_Active(true)
{
}

Component::Component(Entity* entity)
    : m_Entity(entity), m_Active(true)
{
}

void Component::UpdateP()
{

    Update();
}

void Component::SetActive(const bool& active)
{
    m_Active = active;
}
