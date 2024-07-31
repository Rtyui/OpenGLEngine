#include "rendering/internal/VertexArray.h"

VertexArray::VertexArray() :
    m_VertexBuffer(nullptr)
{
    glGenVertexArrays(1, &m_RendererID);
    glBindVertexArray(m_RendererID);
}

VertexArray::~VertexArray()
{
    delete m_VertexBuffer;
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddVertexBuffer(VertexBuffer* vb)
{
    vb->Bind();
    VertexBufferLayout* layout = vb->GetLayout();
    const auto& elements = layout->GetElements();
    unsigned offset = 0;
    for (unsigned i = 0; i < elements.size(); ++i)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout->GetStride(), (const void*)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
    m_VertexBuffer = vb;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}
