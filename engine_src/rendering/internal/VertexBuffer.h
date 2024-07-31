#pragma once
#include "VertexBufferLayout.h"

#include <memory>
#include <vector>

class VertexBuffer
{

private:

    unsigned m_RendererID;
    VertexBufferLayout* m_Layout;
    unsigned m_CurrentSize;
    unsigned m_MaxSize;

public:

    VertexBuffer(const std::vector<float>& data, VertexBufferLayout* layout);
    VertexBuffer(VertexBufferLayout* layout, const unsigned& size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    void Empty();
    void PopData(const unsigned& dataCount);
    void PushData(const std::vector<float>& vertices);
    void SetData(const std::vector<float>& vertices);

    inline VertexBufferLayout* GetLayout() const { return m_Layout; }
};
