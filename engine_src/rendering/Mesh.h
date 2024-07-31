#pragma once
#include "rendering/internal/VertexArray.h"
#include "rendering/internal/IndexBuffer.h"

#include <memory>
#include <string>

class Mesh
{
private:
    std::string m_Name;
    VertexArray* m_VertexArray;
    IndexBuffer* m_IndexBuffer;

public:
    Mesh(const std::string& name, VertexArray* vertexArray, IndexBuffer* indexBuffer);
    ~Mesh();

    void Bind() const;
    void Unbind() const;

    void Empty();
    void PopData(const unsigned& verticesCount, const unsigned& indicesCount);
    void PushData(const std::vector<float>& vertices, const std::vector<unsigned>& indices);
    void SetData(const std::vector<float>& vertices, const std::vector<unsigned>& indices);
    void SetData(const std::vector<float>& vertices);
    void SetData(const std::vector<unsigned>& indices);

    inline const IndexBuffer* GetIndexBuffer() const { return m_IndexBuffer; }
    inline const std::string& GetName() const { return m_Name; }

    static Mesh* CreateMesh(const std::vector<float>& vertices, VertexBufferLayout* layout,
        const std::vector<unsigned>& indices, const std::string& name);
    static Mesh* CreateEmptyMesh(const unsigned& verticesSize, VertexBufferLayout* layout,
        const unsigned& indicesSize, const std::string& name);
};
