#include "entities/Scene.h"
#include "interface/Debug.h"
#include "components/ComponentsFactory.h"

Scene* Scene::s_CurrentScene = nullptr;

Scene::Scene() :
    m_Name("default")
{
}

Scene::~Scene()
{
    for (auto* entityIt : m_Entities)
    {
        delete entityIt;
    }
}

void Scene::Load()
{
    for (auto& entityIt : m_Entities)
    {
        entityIt->Load();
    }
    s_CurrentScene = this;
}

void Scene::Unload(Renderer* renderer)
{
    for (auto& entityIt : m_Entities)
    {
        entityIt->Unload();
    }
}

void Scene::Update()
{
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