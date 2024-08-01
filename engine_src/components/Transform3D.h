#pragma once
#include "components/Transform.h"

#include <vendor/glm/glm.hpp>

class Transform3D : public Transform
{
private:

    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

public:

    void IncreasePosition(glm::vec3 delta) override;
    void IncreaseRotation(glm::vec3 delta) override;

    void IncreasePosition(glm::vec2 delta) override;
    void IncreaseRotation(float delta) override;

    virtual bool IsScreenPointOn(glm::vec2 position) { return false; };

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);

private:

    Transform3D();
    Transform3D(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

    void UpdateTransformationMatrix();

};

