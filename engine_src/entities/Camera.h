#pragma once
#include "interface/Display.h"

#include <vendor/glm/glm.hpp>
#include <memory>
#include <vendor/pugixml/pugixml.hpp>

class Camera
{
    friend class Scene;
private:
    std::string m_Name;
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Forward;
    glm::vec3 m_Right;

    glm::vec3 m_LastPosition;
    glm::vec3 m_LastRotation;
    bool m_UpdatedRotation;

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_UIProjectionMatrix;

    float m_Fov;
    float m_Aspect;
    float m_NearPlane;
    float m_FarPlane;
public:
    Camera();
    Camera(glm::vec3 position);
    ~Camera();
    void Update();

    inline const glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
    inline const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

    static Camera* CreateFromXMLNode(const pugi::xml_node& node);
private:
    void UpdateProjectionMatrix(bool updateUBO);
    void UpdateViewMatrix(bool updateUBO);
    void Move();
};
