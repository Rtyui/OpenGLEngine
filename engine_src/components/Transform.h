#pragma once
#include "components/Component.h"

#include <vendor/glm/glm.hpp>

class Transform : public Component
{
protected:

    glm::mat4 m_TransformationMatrix;
    glm::mat4 m_NoScaleTM;

public:

    virtual void Update() {};

    virtual void IncreasePosition(glm::vec3 delta) = 0;
    virtual void IncreaseRotation(glm::vec3 delta) = 0;

    virtual void IncreasePosition(glm::vec2 delta) = 0;
    virtual void IncreaseRotation(float delta) = 0;

    inline const glm::mat4 GetTransformationMatrix() const { return m_TransformationMatrix; }
    inline const glm::mat4 GetNoScaleTM() const { return m_NoScaleTM; }

};

