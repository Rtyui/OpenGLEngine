#pragma once
#include "interface/Display.h"
#include "components/Component.h"

#include <vendor/glm/glm.hpp>
#include <memory>
#include <vendor/pugixml/pugixml.hpp>

class Camera : public Component
{
    friend class Scene;
private:
    glm::vec3 m_Forward;
    glm::vec3 m_Right;

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_UIProjectionMatrix;

    float m_Fov;
    float m_Aspect;
    float m_NearPlane;
    float m_FarPlane;
public:
    Camera(const float fov, const float near, const float far);
    ~Camera();
    void Load() override;
    void Update();

    inline const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
    inline const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);
private:
    void UpdateProjectionMatrix(bool updateUBO);
    void UpdateViewMatrix(bool updateUBO);
    void Move();

};
