#include "interface/events/CharEvent.h"
#include "interface/events/EventListener.h"

void CharEvent::GetHandled(EventListener* eventListener)
{
    eventListener->HandleEvent(this);
}

CharEvent::CharEvent(unsigned codepoint) :
    m_Codepoint(codepoint)
{
}

Event::EventType CharEvent::GetType()
{
    return Event::EventType::CharEvent;
}
