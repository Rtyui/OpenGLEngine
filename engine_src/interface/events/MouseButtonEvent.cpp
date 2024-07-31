#include "interface/events/MouseButtonEvent.h"
#include "interface/events/EventListener.h"

void MouseButtonEvent::GetHandled(EventListener* eventListener)
{
    eventListener->HandleEvent(this);
}

MouseButtonEvent::MouseButtonEvent(int button, int action, int mods) :
    m_Button(button), m_Action(action), m_Mods(mods)
{
}

Event::EventType MouseButtonEvent::GetType()
{
    return Event::EventType::MouseButtonEvent;
}
