#include "rendering/Renderable.h"

Renderable::Renderable() :
    m_Layer(0)
{
}

Renderable::Renderable(const unsigned& layer) :
    m_Layer(layer)
{
}
