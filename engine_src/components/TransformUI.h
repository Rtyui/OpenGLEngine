#pragma once
#include "components/Transform.h"
class TransformUI : public Transform
{
public:

    bool IsScreenPointOn(glm::vec2 position) override;

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);
    static Component* Create(const glm::vec3& position, const float& rotation, const glm::vec3& size);

private:

    TransformUI();
    TransformUI(const glm::vec3& position, const float& rotation, const glm::vec3& size);

    void UpdateTransformationMatrix() override;
};

