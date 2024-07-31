#pragma once

#include <vector>
#include <GL/glew.h>
#include <iostream>
#include <memory>

struct VertexBufferElement
{
    std::string name;
    unsigned count;
    unsigned type;
    unsigned normalized;

    static unsigned GetSizeOfType(unsigned type)
    {
        switch (type)
        {
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned m_Stride;
public:
    VertexBufferLayout() : m_Stride(0) {}

    template<typename T>
    void Push(const std::string& name, unsigned count)
    {
        std::cerr << "Unknown type" << std::endl;
    }

    template<>
    void Push<float>(const std::string& name, unsigned count)
    {
        m_Elements.push_back({ name, count, GL_FLOAT, GL_FALSE });
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned>(const std::string& name, unsigned count)
    {
        m_Elements.push_back({ name, count, GL_UNSIGNED_INT, GL_FALSE });
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(const std::string& name, unsigned count)
    {
        m_Elements.push_back({ name, count, GL_UNSIGNED_BYTE, GL_TRUE });
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    bool HasElement(const std::string& name) const
    {
        for (auto& element : m_Elements)
        {
            if (element.name == name)
            {
                return true;
            }
        }
        return false;
    }

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline const unsigned GetStride() const { return m_Stride; }
};
