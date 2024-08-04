#include "components/Renderable.h"
#include "Renderable.h"
#include "rendering/Renderer.h"

Renderable::Renderable() :
    m_Layer(0)
{
}

Renderable::Renderable(const unsigned& layer) :
    m_Layer(layer)
{
}

void Renderable::Load()
{
    Renderer::Singleton()->Submit(this);
}

void Renderable::Unload()
{
    Renderer::Singleton()->UnSubmit(this);
}
