#pragma once
#include "components/Transform.h"

#include <vendor/glm/glm.hpp>

class Transform3D : public Transform
{
private:

    glm::vec3 m_LastPosition;
    glm::vec3 m_LastRotation;

public:

    virtual void Update() override;

    virtual bool IsScreenPointOn(glm::vec2 position) { return false; };

    virtual const glm::vec3 GetPosition() const { return m_Position; };
    virtual const glm::vec3 GetRotation() const { return m_Rotation; };

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);

private:

    Transform3D();
    Transform3D(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

    virtual void UpdateTransformationMatrix() override;

};

