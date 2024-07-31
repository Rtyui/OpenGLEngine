#include "VertexBuffer.h"
#include "interface/Debug.h"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(const std::vector<float>& data, VertexBufferLayout* layout) :
    m_Layout(layout), m_CurrentSize((unsigned)data.size()), m_MaxSize((unsigned)data.size())
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(VertexBufferLayout* layout, const unsigned& size) :
    m_Layout(layout), m_CurrentSize(0), m_MaxSize(size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
    delete m_Layout;
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Empty()
{
    m_CurrentSize = 0;
}

void VertexBuffer::PopData(const unsigned& dataCount)
{
    if (m_CurrentSize < dataCount)
    {
        Debug::Log(Debug::Error, "Trying to remove more buffer elements that exists!");
        return;
    }
    m_CurrentSize -= dataCount;
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, m_CurrentSize * sizeof(float), dataCount * sizeof(float), nullptr);
}

void VertexBuffer::PushData(const std::vector<float>& vertices)
{
    if ((m_CurrentSize + vertices.size()) > m_MaxSize)
    {
        Debug::Log(Debug::Error, "Max size reached!");
        return;
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, m_CurrentSize * sizeof(float), vertices.size() * sizeof(float), vertices.data());
    m_CurrentSize += (unsigned)vertices.size();
}

void VertexBuffer::SetData(const std::vector<float>& vertices)
{
    if (vertices.size() > m_MaxSize)
    {
        Debug::Log(Debug::Error, "Max size reached!");
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    m_CurrentSize = (unsigned)vertices.size();
}
