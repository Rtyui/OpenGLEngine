#pragma once

#include <GL/glew.h>
#include <memory>
#include <vector>

class IndexBuffer
{
private:
    unsigned m_RendererID;

    unsigned m_CurrentSize;
    unsigned m_MaxSize;

public:
    IndexBuffer(const std::vector<unsigned>& data);
    IndexBuffer(const unsigned& size);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    void Empty();
    void PopData(const unsigned& dataCount);
    void PushData(const std::vector<unsigned>& indices);
    void SetData(const std::vector<unsigned>& indices);

    inline unsigned GetCount() const { return m_CurrentSize; }
};
