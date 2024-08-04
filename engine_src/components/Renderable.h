#pragma once

#include "components/Component.h"

class Renderable : public Component
{
private:

    unsigned m_Layer;

public:

    inline const unsigned& GetLayer() const { return m_Layer; }

protected:

    Renderable();
    Renderable(const unsigned& layer);

    virtual void Load() override;
    virtual void Unload() override;
    virtual void Update() override = 0;
};

