#pragma once

#include "interface/events/Event.h"

class CharEvent : public Event
{
private:

    int m_Codepoint;

public:

    inline const int& GetCodepoint() { return m_Codepoint; }

    virtual void GetHandled(EventListener* eventListener) override;

public:

    CharEvent(unsigned codepoint);

    Event::EventType GetType() override;
};

