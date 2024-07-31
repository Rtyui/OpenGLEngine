#include "entities/Scene.h"
#include "interface/Debug.h"
#include "components/ComponentsFactory.h"

Scene* Scene::s_CurrentScene = nullptr;

Scene::Scene() :
    m_Name("default"), m_Camera(nullptr), m_Light(nullptr)
{
}

Scene::~Scene()
{
    if (m_Camera)
    {
        delete m_Camera;
    }
    if (m_Light)
    {
        delete m_Light;
    }
    for (auto* entityIt : m_Entities)
    {
        delete entityIt;
    }
}

void Scene::Load()
{
    std::vector<Renderable*> renderables;
    for (auto& entityIt : m_Entities)
    {
        entityIt->GetComponents<Renderable>(renderables);
    }

    for (Renderable* renderable : renderables)
    {
        renderable->GetSubmitted();
    }

    m_Light->Load();
    m_Camera->UpdateProjectionMatrix(true);
    s_CurrentScene = this;
}

void Scene::Unload(Renderer* renderer)
{
    std::vector<Renderable*> renderables;
    for (auto& entityIt : m_Entities)
    {
        entityIt->GetComponents<Renderable>(renderables);
    }

    for (Renderable* renderable : renderables)
    {
        renderable->GetSubmitted();
    }
}

void Scene::Update()
{
    m_Camera->Update();
    for (Entity* entity : m_Entities)
    {
        entity->Update();
    }
}

Entity* Scene::GetEntity(const std::string& name)
{
    for (auto* entity : m_Entities)
    {
        if (entity->GetName() == name)
        {
            return entity;
        }
    }
    return nullptr;
}


Scene* Scene::CreateFromXMLFile(const std::string& name, const std::string& filePath)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());
    std::string logPrefix;
    Scene* scene = new Scene();

    if (!result)
    {
        Debug::Log(Debug::Warning, "Could not load xml file " + filePath);
        delete scene;
        return nullptr;
    }

    pugi::xml_node mainChild = doc.child("Scene");
    if (mainChild.empty())
    {
        Debug::Log(Debug::Warning, "File '" + filePath + "' first child not 'Scene'");
        delete scene;
        return nullptr;
    }

    std::string sceneName(mainChild.attribute("name").value());
    if (sceneName != name)
    {
        Debug::Log(Debug::Error, "Name mismatch '" + name + "' != '" + sceneName + "'");
        delete scene;
        return nullptr;
    }
    scene->m_Name = name;

    for (pugi::xml_node child : mainChild.children())
    {
        std::string childName(child.name());

        if (childName == "Entity")
        {
            LoadXMLEntity(scene, child);
        }
        else if (childName == "Light")
        {
            LoadXMLLight(scene, child);
        }
        else if (childName == "Camera")
        {
            LoadXMLCamera(scene, child);
        }
        else
        {
            Debug::Log(Debug::Warning, "Unknown child type name '" + childName + "'!");
        }
    }

    return scene;
}

void Scene::LoadXMLEntity(Scene* scene, const pugi::xml_node& node)
{
    Entity* entity = Entity::CreateFromXMLNode(node);
    if (!entity)
    {
        return;
    }

    for (pugi::xml_node child : node.children())
    {
        std::string childName(child.name());

        if (Component* component = ComponentsFactory::CreateComponent(childName, child, entity))
        {
            entity->AddComponent(component);
        }
    }

    if (!entity->GetTransform())
    {
        Debug::Log(Debug::Error, "Entity '" + entity->GetName() + "' does not have transform!");
        delete entity;
        return;
    }
    scene->m_Entities.push_back(entity);
}

void Scene::LoadXMLLight(Scene* scene, const pugi::xml_node& node)
{
    Light* light = Light::CreateFromXMLNode(node);
    if (!light)
    {
        return;
    }
    if (scene->m_Light)
    {
        delete scene->m_Light;
    }
    scene->m_Light = light;
}

void Scene::LoadXMLCamera(Scene* scene, const pugi::xml_node& node)
{
    Camera* camera = Camera::CreateFromXMLNode(node);
    if (!camera)
    {
        return;
    }
    if (scene->m_Camera)
    {
        delete scene->m_Camera;
    }
    Debug::Log(Debug::Info, "parsed camera");
    scene->m_Camera = camera;
}
