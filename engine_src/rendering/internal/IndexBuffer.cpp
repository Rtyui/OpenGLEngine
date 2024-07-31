#include "rendering/internal/IndexBuffer.h"
#include "interface/Debug.h"

#include <iostream>

IndexBuffer::IndexBuffer(const std::vector<unsigned>& data) :
    m_CurrentSize((unsigned)data.size()), m_MaxSize((unsigned)data.size())
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const unsigned& size) :
    m_CurrentSize(0), m_MaxSize(size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Empty()
{
    m_CurrentSize = 0;
}

void IndexBuffer::PopData(const unsigned& dataCount)
{
    if (m_CurrentSize < dataCount)
    {
        Debug::Log(Debug::Error, "Trying to remove more buffer elements that exists!");
        return;
    }
    m_CurrentSize -= dataCount;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_CurrentSize * sizeof(unsigned), dataCount * sizeof(unsigned), nullptr);
}

void IndexBuffer::PushData(const std::vector<unsigned>& indices)
{
    if ((m_CurrentSize + indices.size()) > m_MaxSize)
    {
        Debug::Log(Debug::Error, "Max size reached!");
        return;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_CurrentSize * sizeof(unsigned), indices.size() * sizeof(unsigned), indices.data());
    m_CurrentSize += (unsigned)indices.size();
}

void IndexBuffer::SetData(const std::vector<unsigned>& indices)
{
    if (indices.size() > m_MaxSize)
    {
        Debug::Log(Debug::Error, "Max size reached!");
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned), indices.data());
    m_CurrentSize = (unsigned)indices.size();
}
