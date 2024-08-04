#include "components/MeshRender.h"
#include "Resources.h"
#include "interface/Debug.h"
#include "rendering/Renderer.h"
#include "utils/XMLUtils.h"

MeshRender::MeshRender(Mesh* mesh, Material* material)
    : m_Mesh(mesh), m_Material(material)
{
}

Component* MeshRender::CreateFromXMLNode(const pugi::xml_node& node, Entity* entity)
{
    Mesh* mesh = nullptr;
    Material* material = nullptr;
    if (!ParseMeshFromXMLNode(node, &mesh, true) ||
        !ParseMaterialFromXMLNode(node, &material, true))
    {
        return nullptr;
    }

    return new MeshRender(mesh, material);
}
