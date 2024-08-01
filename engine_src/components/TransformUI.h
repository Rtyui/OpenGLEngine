#pragma once
#include "components/Transform.h"
class TransformUI : public Transform
{
private:

    glm::vec2 m_Position;
    float m_Rotation;
    glm::vec2 m_Size;

public:

    void IncreasePosition(glm::vec3 delta) override;
    void IncreaseRotation(glm::vec3 delta) override;

    void IncreasePosition(glm::vec2 delta) override;
    void IncreaseRotation(float delta) override;

    bool IsScreenPointOn(glm::vec2 position) override;

    inline const glm::vec2& GetPosition() const { return m_Position; }
    inline const glm::vec2& GetSize() const { return m_Size; }


    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);
    static Component* Create(const glm::vec2& position, const float& rotation, const glm::vec2& size);

private:

    TransformUI();
    TransformUI(const glm::vec2& position, const float& rotation, const glm::vec2& size);

    void UpdateTransformationMatrix();
};

