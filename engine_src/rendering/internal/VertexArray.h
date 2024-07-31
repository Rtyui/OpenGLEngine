#pragma once
#include "VertexBuffer.h"
#include "rendering/internal/IndexBuffer.h"

#include <memory>

class VertexArray
{
private:
    unsigned m_RendererID;

    VertexBuffer* m_VertexBuffer;
public:

    VertexArray();
    ~VertexArray();

    void AddVertexBuffer(VertexBuffer* vb);

    void Bind() const;
    void Unbind() const;

    inline VertexBuffer* GetVertexBuffer() const { return m_VertexBuffer; }
};
