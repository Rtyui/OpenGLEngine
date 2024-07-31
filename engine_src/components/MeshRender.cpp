#include "components/MeshRender.h"
#include "Resources.h"
#include "interface/Debug.h"
#include "rendering/Renderer.h"

MeshRender::MeshRender(Mesh* mesh, Material* material)
    : m_Mesh(mesh), m_Material(material)
{
}

void MeshRender::GetSubmitted()
{
    Renderer::Singleton()->Submit(this);
}

void MeshRender::GetUnSubmitted()
{
    Renderer::Singleton()->UnSubmit(this);
}

Component* MeshRender::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    const std::string& meshName = node.attribute("mesh").value();
    Mesh* mesh = Resources::Singleton()->GetMesh(meshName);
    if (!mesh)
    {
        Debug::Log(Debug::Error, "Invalid mesh '" + meshName + "'!");
        return nullptr;
    }

    const std::string& materialName = node.attribute("material").value();
    Material* material = Resources::Singleton()->GetMaterial(materialName);
    if (!material)
    {
        Debug::Log(Debug::Error, "Invalid material '" + materialName + "'!");
        return nullptr;
    }

    return new MeshRender(mesh, material);
}
