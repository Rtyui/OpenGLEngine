#pragma once
#include "components/Component.h"

#include <vendor/glm/glm.hpp>

class Transform : public Component
{
protected:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

    bool m_PositionChanged;
    bool m_RotationChanged;

    glm::mat4 m_NoScaleTM;
    glm::mat4 m_TransformationMatrix;

protected:

    Transform();
    Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

public:

    virtual void Update() {};

    virtual bool HasPositionChanged() { return m_PositionChanged; }
    virtual bool HasRotationChanged() { return m_RotationChanged; }

    const glm::vec3 GetPosition() const { return m_Position; };
    const glm::vec3 GetRotation() const { return m_Rotation; };
    const glm::vec3 GetScale() const { return m_Scale; };

    void IncreasePosition(glm::vec3 delta);
    void IncreaseRotation(glm::vec3 delta);

    virtual void UpdateTransformationMatrix() = 0;
    virtual bool IsScreenPointOn(glm::vec2 position) = 0;

    inline const glm::mat4 GetTransformationMatrix() const { return m_TransformationMatrix; }
    inline const glm::mat4 GetNoScaleTM() const { return m_NoScaleTM; }

};

