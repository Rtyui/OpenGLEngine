#pragma once

#include "interface/events/Event.h"

class EventListener;

class KeyEvent : public Event
{
private:

    int m_Key;
    int m_Scancode;
    int m_Action;
    int m_Mods;

public:

    inline const int& GetKey() { return m_Key; }
    inline const int& GetScancode() { return m_Scancode; }
    inline const int& GetAction() { return m_Action; }
    inline const int& GetMods() { return m_Mods; }

    virtual void GetHandled(EventListener* eventListener) override;

public:

    KeyEvent(int key, int scancode, int action, int mods);

    Event::EventType GetType() override;

};

