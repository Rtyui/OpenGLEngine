#pragma once
#include <vector>

#include "components/Camera.h"
#include "components/Light.h"
#include "entities/Entity.h"
#include "rendering/Renderer.h"

class Scene
{
private:
    static Scene* s_CurrentScene;

    std::string m_Name;

    std::vector<Entity*> m_Entities;

public:

    Scene();
    ~Scene();

    void Load();
    void Unload(Renderer* renderer);

    void Update();

    Entity* GetEntity(const std::string& name);

    static Scene* CreateFromXMLFile(const std::string& name, const std::string& filePath);

    inline const std::string& GetName() { return m_Name; }
    inline static Scene* GetCurrentScene() { return s_CurrentScene; }

private:

    static void LoadXMLEntity(Scene* scene, const pugi::xml_node& node);

};

