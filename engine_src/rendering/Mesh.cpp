#include "rendering/Mesh.h"

Mesh::Mesh(const std::string& name, VertexArray* vertexArray, IndexBuffer* indexBuffer)
    : m_Name(name), m_VertexArray(vertexArray), m_IndexBuffer(indexBuffer)
{
}

Mesh::~Mesh()
{
    delete m_VertexArray;
    delete m_IndexBuffer;
}

void Mesh::Bind() const
{
    m_VertexArray->Bind();
}

void Mesh::Unbind() const
{
    m_VertexArray->Unbind();
}

void Mesh::Empty()
{
    m_VertexArray->Bind();
    m_VertexArray->GetVertexBuffer()->Empty();
    m_IndexBuffer->Empty();
}

void Mesh::PopData(const unsigned& verticesCount, const unsigned& indicesCount)
{
    m_VertexArray->Bind();
    m_VertexArray->GetVertexBuffer()->PopData(verticesCount);
    m_IndexBuffer->PopData(indicesCount);
}

void Mesh::PushData(const std::vector<float>& vertices, const std::vector<unsigned>& indices)
{
    m_VertexArray->Bind();
    m_VertexArray->GetVertexBuffer()->PushData(vertices);
    m_IndexBuffer->PushData(indices);
}

void Mesh::SetData(const std::vector<float>& vertices, const std::vector<unsigned>& indices)
{
    m_VertexArray->Bind();
    m_VertexArray->GetVertexBuffer()->SetData(vertices);
    m_IndexBuffer->SetData(indices);
}

void Mesh::SetData(const std::vector<float>& vertices)
{
    m_VertexArray->Bind();
    m_VertexArray->GetVertexBuffer()->SetData(vertices);
}

void Mesh::SetData(const std::vector<unsigned>& indices)
{
    m_VertexArray->Bind();
    m_IndexBuffer->SetData(indices);
}

Mesh* Mesh::CreateMesh(const std::vector<float>& vertices,
    VertexBufferLayout* layout,
    const std::vector<unsigned>& indices,
    const std::string& name)
{
    VertexArray* va = new VertexArray();
    VertexBuffer* vb = new VertexBuffer(vertices, layout);

    va->AddVertexBuffer(vb);

    IndexBuffer* ib = new IndexBuffer(indices);
    va->Unbind();
    return new Mesh(name, va, ib);
}

Mesh* Mesh::CreateEmptyMesh(const unsigned& verticesSize, VertexBufferLayout* layout, const unsigned& indicesSize, const std::string& name)
{
    VertexArray* va = new VertexArray();
    VertexBuffer* vb = new VertexBuffer(layout, verticesSize);

    va->AddVertexBuffer(vb);

    IndexBuffer* ib = new IndexBuffer(indicesSize);
    va->Unbind();
    return new Mesh(name, va, ib);
}
