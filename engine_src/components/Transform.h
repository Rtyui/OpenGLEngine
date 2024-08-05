#pragma once
#include "components/Component.h"

#include <vendor/glm/glm.hpp>

class Transform : public Component
{
private:

    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

    glm::vec3 m_LastPosition;
    glm::vec3 m_LastRotation;

    bool m_PositionChanged;
    bool m_RotationChanged;

    bool m_UI;

    glm::mat4 m_NoScaleTM;
    glm::mat4 m_TransformationMatrix;

protected:

    Transform();
    Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const bool& ui);

public:

    void Update();

    inline bool HasPositionChanged() const { return m_PositionChanged; }
    inline bool HasRotationChanged() const { return m_RotationChanged; }

    inline const glm::vec3 GetPosition() const { return m_Position; };
    inline const glm::vec3 GetRotation() const { return m_Rotation; };
    inline const glm::vec3 GetScale() const { return m_Scale; };

    void IncreasePosition(glm::vec3 delta);
    void IncreaseRotation(glm::vec3 delta);

    void UpdateTransformationMatrix();
    bool IsScreenPointOn(glm::vec2 position) const;

    inline const glm::mat4 GetTransformationMatrix() const { return m_TransformationMatrix; }
    inline const glm::mat4 GetNoScaleTM() const { return m_NoScaleTM; }

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);
    static Component* Create(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& size, const bool& ui);

};

