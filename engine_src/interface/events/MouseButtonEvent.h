#pragma once
#include "interface/events/Event.h"

class MouseButtonEvent : public Event
{
private:

    int m_Button;
    int m_Action;
    int m_Mods;

public:

    inline const int& GetKey() { return m_Button; }
    inline const int& GetAction() { return m_Action; }
    inline const int& GetMods() { return m_Mods; }

    virtual void GetHandled(EventListener* eventListener) override;

public:

    MouseButtonEvent(int button, int action, int mods);

    Event::EventType GetType() override;
};

