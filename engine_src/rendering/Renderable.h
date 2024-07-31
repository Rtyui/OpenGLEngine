#pragma once

class Renderer;

class Renderable
{
private:

    unsigned m_Layer;

public:

    virtual void GetSubmitted() = 0;
    virtual void GetUnSubmitted() = 0;

    inline const unsigned& GetLayer() const { return m_Layer; }

protected:

    Renderable();
    Renderable(const unsigned& layer);
};

