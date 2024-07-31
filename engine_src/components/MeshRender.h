#pragma once
#include "components/Component.h"
#include "rendering/Mesh.h"
#include "rendering/Material.h"
#include "rendering/Renderable.h"

class Renderer;

class MeshRender : public Component, public Renderable
{
private:
    Mesh* m_Mesh;
    Material* m_Material;

public:
    MeshRender() = default;
    MeshRender(Mesh* mesh, Material* material);

    virtual void GetSubmitted() override;
    virtual void GetUnSubmitted() override;
    virtual void Update() {};

    inline Mesh* GetMesh() { return m_Mesh; }
    inline Material* GetMaterial() { return m_Material; }

    static Component* CreateFromXMLNode(const pugi::xml_node& node, Entity* entity);
};
