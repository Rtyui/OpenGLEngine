#pragma once
#include "interface/events/Event.h"

class WindowSizeEvent : public Event
{
private:

    int m_Width;
    int m_Height;

public:

    inline const int& GetWidth() const { return m_Width; }
    inline const int& GetHeight() const { return m_Height; }

    virtual void GetHandled(EventListener* eventListener) override;

public:

    WindowSizeEvent(int width, int height);

    Event::EventType GetType() override;
};

